#ifndef CSSRULES_H
#define CSSRULES_H

#include "global.h"
#include <map>
#include <string>
#include <vector>

class css_node
{
    std::vector<std::wstring> _selectors;
    std::map<std::wstring, std::wstring> _rules;
public:
    css_node();
    void add_rule(const std::wstring &name, const std::wstring &value);
    void set_attr(const std::wstring &name, const std::wstring &value);
    void add_selector(const std::wstring &name);
    bool has_selector(const std::wstring &name);
    std::vector<std::wstring> selectors() const {
        return _selectors;
    }
    std::wstring to_string(print_type type = print_type::compact) const;
};

class css_doc : public std::vector<css_node*>
{
public:
    std::wstring to_string(print_type type = print_type::compact) const;
};

#endif // CSSRULES_H
