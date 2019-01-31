#include "cssparser.h"
#include "cssrules.h"
#include "htmltag.h"
#include "string_helper.h"
#include "tokenparser.h"

#include <iostream>
#include <algorithm>

bool html_tag::hasCloseTag() const
{
    return _hasCloseTag;
}

void html_tag::setHasCloseTag(bool hasCloseTag)
{
    _hasCloseTag = hasCloseTag;
}

std::vector<html_node *> html_tag::childs() const
{
    return _childs;
}

html_tag::html_tag() : html_node (), _css(new css_node)
{

}

html_tag::~html_tag()
{
    for (html_node *child : _childs)
        delete child;
}

std::wstring html_tag::id()
{
    return _attributes[L"id"];
}

std::wstring html_tag::attr(const std::wstring &name)
{
    return _attributes[name];
}

void html_tag::set_attr(const std::wstring &name, const std::wstring &value)
{
    auto n = name;
    string_helper::tolower(n);
    //TODO: parse css
    if (n == L"style") {
        css_parser p;
        p.setText(value);
        p.parse_tokens();
        size_t i = 0;
        p.parse_rules(_css, i);
    } else if (n == L"class") {
//        _classes = value.split(" ");
    }

    _attributes[name] = value;
}

void html_tag::add_class(const std::wstring &name)
{
    if (std::find(_classes.begin(), _classes.end(), name) != _classes.end())
        _classes.push_back(name);
}

void html_tag::remove_class(const std::wstring &name)
{
    _classes.erase(std::remove(_classes.begin(), _classes.end(), name),
                   _classes.end());
}

bool html_tag::has_class(const std::wstring &name) const
{
    return (std::find(_classes.begin(), _classes.end(), name) != _classes.end());
}

void html_tag::add_child(html_node *child)
{
    _childs.push_back(child);
    child->set_parent(this);
}


std::wstring html_tag::outter_html(print_type type)
{
    std::wstring html = L"<" + name;
    std::map<std::wstring, std::wstring>::iterator it;
    for(it = _attributes.begin(); it != _attributes.end(); ++it)
        html.append(L" " + it->first + L"=\"" + it->second + L"\"");

    if (_hasCloseTag)
        html.append(L">");
    else
        html.append(L" />");

    if (type == print_type::formatted)
        html.append(L"\n");

    html.append(inner_html(type));

    if (type == print_type::formatted)
        html.append(L"\n");
    if (_hasCloseTag) {
        html.append(L"</" + name + L">");
        if (type == print_type::formatted)
            html.append(L"\n");
    }

    return html;
}

std::wstring html_tag::inner_html(print_type type) const
{
    std::wstring html;
    std::vector<html_node*>::const_iterator i;
    for (i = _childs.cbegin(); i != _childs.cend(); ++i)
        html.append((*i)->outter_html(type));

    return html;
}

std::wstring html_tag::inner_text() const
{
    std::wstring html;
    std::vector<html_node*>::const_iterator i;
    for (i = _childs.cbegin(); i != _childs.cend(); ++i) {
        std::wstring buffer = (*i)->inner_text();
//        trim(buffer);
        if (buffer != L"") {
            if (buffer.substr(buffer.length() - 1, 1) != L" ")
                buffer.append(L" ");
            html.append(buffer);
        }
    }

    return html;
}

html_node::html_node()
{

}

html_node::~html_node()
{

}

html_node *html_node::parent() const
{
    return _parent;
}

void html_node::set_parent(html_node *parent)
{
    _parent = parent;
}

text_node::text_node() : html_node ()
{

}

std::wstring text_node::text() const
{
    return _text;
}

void text_node::setText(const std::wstring &text)
{
    _text = text;
    string_helper::replace(_text, L"\r", L" ");
    string_helper::replace(_text, L"\n", L" ");
    std::wcout << L"transformed: " << _text;
}

std::wstring text_node::outter_html(print_type type)
{
    return _text;
}

std::wstring text_node::inner_text() const
{
    return  _text;
}

//css_doc style_tag::rules() const
//{
//    return _rules;
//}

//void style_tag::setRules(const css_doc &rules)
//{
//    _rules = rules;
//}

std::wstring style_tag::inner_html(print_type type) const
{
    if (rules.size())
        return rules.to_string(type);
    else
        return std::wstring();
}

style_tag::style_tag() : html_tag()
{

}

void style_tag::add_child(html_node *child)
{
    text_node *tn = dynamic_cast<text_node*>(child);
    if (tn) {
        css_parser cp;
        cp.setText(tn->text());
        cp.parse();
        rules = cp.doc;
        std::wcout << L"new css" << cp.doc.to_string() << rules.size() << L">>>";
    } else {
        std::cout << "Appending non-text node to style tag was not allowed";
    }
}
