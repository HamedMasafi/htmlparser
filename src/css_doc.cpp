#include "css_doc.h"

#include <algorithm>
#include "string_renderer.h"

css_node::css_node()
{

}

void css_node::add_rule(const std::wstring &name, const std::wstring &value)
{
    _rules[name] = value;
}

std::wstring css_node::to_string(print_type type) const
{
    std::wstring ret;

    for (auto it = _rules.begin(); it != _rules.end(); ++it) {
        if (type == print_type::formatted)
            ret.append(L"\n    ");

        if (ret.size() && type == print_type::compact)
            ret.append(L";");

        if (type == print_type::formatted) {
            ret.append(it->first + L": " + it->second);
            ret.append(L";");
        } else {
            ret.append(it->first + L":" + it->second);
        }
    }
    std::wstring selectors;
    for (std::wstring s : _selectors) {
        if (selectors.size())
            selectors.append(L", ");
        selectors.append(s);
    }

    if (type == print_type::formatted)
        ret.append(L"\n");
    return selectors + L"{" + ret + L"}";
}

void css_node::append(string_renderer &r)
{
    bool f = true;
    for (std::wstring s : _selectors) {
        if (!f)
            r.append(L", ");
        f = false;
        r.append(s);
    }
    r.append(L" {");
    r.indent();
    for (auto it = _rules.begin(); it != _rules.end(); ++it) {
        r.new_line();
        r.append(it->first);
        r.append(L":");
        r.space();
        r.append(it->second);
        r.append(L";");
    }
    r.unindent();
    r.new_line();
    r.append(L"}");
    r.new_line();
}

std::wstring css_doc::to_string(print_type type) const
{
    string_renderer r(type);
    for (auto i = cbegin(); i != cend(); ++i)
        (*i)->append(r);
    return r.to_string();
}

void css_node::set_attr(const std::wstring &name, const std::wstring &value)
{
    _rules[name] = value;
}

void css_node::add_selector(const std::wstring &name)
{
    _selectors.push_back(name);
}

bool css_node::has_selector(const std::wstring &name)
{
    return std::any_of(_selectors.begin(), _selectors.end(), [=](std::wstring &s){
       return s == name;
    });
}
