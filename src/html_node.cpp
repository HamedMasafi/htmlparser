#include "css_parser.h"
#include "css_doc.h"
#include "html_node.h"
#include "string_helper.h"
#include "token_parser.h"

#include <iostream>
#include <algorithm>
#include <string_renderer.h>

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
    string_renderer r(type);
    append(r);
    return r.to_string();
}

void html_tag::append(string_renderer &r)
{
    r.new_line();
    append_begin_tag(r);
    if (_has_close_tag)
        r.indent();
    r.new_line();
    append_inner_html(r);
    if (_has_close_tag)
        r.unindent();
    append_end_tag(r);
    r.new_line();
}

void html_tag::append_begin_tag(string_renderer &r)
{
    r.append(L"<" + name);
    std::map<std::wstring, std::wstring>::iterator it;
    for(it = _attributes.begin(); it != _attributes.end(); ++it)
        r.append(L" " + it->first + L"=\"" + it->second + L"\"");

    if (_has_close_tag)
        r.append(L">");
    else
        r.append(L" />");
}

void html_tag::append_inner_html(string_renderer &r)
{
    for (auto child : _childs)
        child->append(r);
}

void html_tag::append_end_tag(string_renderer &r)
{
    if (_has_close_tag) {
        r.new_line();
        r.append(L"</" + name + L">");
    }
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

void text_node::append(string_renderer &r)
{
    r.append(_text);
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
    if (rules.nodes().size())
        return std::wstring();
    else
        return rules.to_string();
}

void style_tag::append(string_renderer &r)
{
    r.append(L"<style>");
    r.indent();
    r.new_line();
    auto nodes = rules.nodes();
    if (nodes.size())
        for (auto i = nodes.cbegin(); i != nodes.cend(); ++i) {
            (*i)->append(r);
            r.new_line();
        }
    r.unindent();
    r.append(L"</style>");
    r.new_line();
}

style_tag::style_tag() : html_tag()
{

}

void style_tag::add_child(html_node *child)
{
    text_node *tn = dynamic_cast<text_node*>(child);
    if (tn) {
        rules.set_text(tn->text());
    } else {
        std::cout << "Appending non-text node to style tag was not allowed";
    }
}
