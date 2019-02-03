#include "css_doc.h"

#include <algorithm>
#include <iostream>

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


std::wstring css_doc::to_string(print_type type) const
{
    std::wstring ret;
    for (auto i = cbegin(); i != cend(); ++i) {
        ret.append((*i)->to_string(type));
        if (type == print_type::formatted)
            ret.append(L"\n");
    }
    return ret;
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
        std::wcout << s << "  <>  " << name << std::endl;
       return s == name;
    });
}
