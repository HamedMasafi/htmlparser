#include "css_parser.h"
#include "css_doc.h"
#include "html_node.h"
#include "string_helper.h"
#include "token_parser.h"

#include <iostream>
#include <algorithm>

bool html_tag::hasCloseTag() const
{
    return _has_close_tag;
}

void html_tag::setHasCloseTag(bool hasCloseTag)
{
    _has_close_tag = hasCloseTag;
}

std::vector<html_node *> html_tag::childs() const
{
    return _childs;
}

std::wstring html_tag::to_string(print_type type)
{
    if (type == print_type::compact)
        return outter_html();

    std::wstring html;// = L"<" + name;
//    std::map<std::wstring, std::wstring>::iterator it;
//    for(it = _attributes.begin(); it != _attributes.end(); ++it)
//        html.append(L" " + it->first + L"=\"" + it->second + L"\"");

//    if (_has_close_tag)
//        html.append(L">");
//    else
//        html.append(L" />");

//    if (type == print_type::formatted)
//        html.append(L"\n");

//    html.append(inner_html(type));

//    if (type == print_type::formatted)
//        html.append(L"\n");
//    if (_has_close_tag) {
//        html.append(L"</" + name + L">");
//        if (type == print_type::formatted)
//            html.append(L"\n");
//    }

    append(html, 0);

    return html;
}

void html_tag::append(std::wstring &html, const size_t &level)
{
    append_begin_tag(html, level);
    append_inner_html(html, level);
    append_end_tag(html, level);
}

void html_tag::append_begin_tag(std::wstring &html, const size_t &level)
{
    html.append(std::wstring(level * 4, ' '));

    html.append(L"<" + name);
    std::map<std::wstring, std::wstring>::iterator it;
    for(it = _attributes.begin(); it != _attributes.end(); ++it)
        html.append(L" " + it->first + L"=\"" + it->second + L"\"");

    html.append(L">\n");
}

void html_tag::append_inner_html(std::wstring &html, const size_t &level)
{
    for (auto child : _childs)
        child->append(html, level + 1);
}

void html_tag::append_end_tag(std::wstring &html, const size_t &level)
{
    html.append(std::wstring(level * 4, ' '));
    html.append(L"</" + name + L">\n");
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
        p.set_text(value);
        auto rules = p.parse_block();
        _attributes = rules;
    } else if (n == L"class") {
        _classes = string_helper::split(value, ' ');
    }

    _attributes[n] = value;
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


std::wstring html_tag::outter_html()
{
    std::wstring html = L"<" + name;
    std::map<std::wstring, std::wstring>::iterator it;
    for(it = _attributes.begin(); it != _attributes.end(); ++it)
        html.append(L" " + it->first + L"=\"" + it->second + L"\"");

    if (_has_close_tag)
        html.append(L">");
    else
        html.append(L" />");

    html.append(inner_html());

    if (_has_close_tag)
        html.append(L"</" + name + L">");

    return html;
}

std::wstring html_tag::inner_html() const
{
    std::wstring html;
    std::vector<html_node*>::const_iterator i;
    for (i = _childs.cbegin(); i != _childs.cend(); ++i)
        html.append((*i)->outter_html());

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
    string_helper::replace(_text, L"\r", L"");
    string_helper::replace(_text, L"\n", L"");
}

std::wstring text_node::outter_html()
{
    return _text;
}

std::wstring text_node::inner_text() const
{
    return  _text;
}

void text_node::append(std::wstring &html, const size_t &level)
{
    html.append(std::wstring(level * 4, ' ') + _text + L"\n");
}

//css_doc style_tag::rules() const
//{
//    return _rules;
//}

//void style_tag::setRules(const css_doc &rules)
//{
//    _rules = rules;
//}

std::wstring style_tag::inner_html() const
{
    if (rules.size())
        return std::wstring();
    else
        return rules.to_string();
}

void style_tag::append(std::wstring &html, const size_t &level)
{
    if (rules.size())
        for (auto i = rules.cbegin(); i != rules.cend(); ++i)
            html.append(std::wstring(level * 4, ' ')
                        + (*i)->to_string(print_type::formatted)
                        + L"\n");
}

style_tag::style_tag() : html_tag()
{

}

void style_tag::add_child(html_node *child)
{
    text_node *tn = dynamic_cast<text_node*>(child);
    if (tn) {
        css_parser cp;
        cp.set_text(tn->text());
        rules = cp.doc;
    } else {
        std::cout << "Appending non-text node to style tag was not allowed";
    }
}
