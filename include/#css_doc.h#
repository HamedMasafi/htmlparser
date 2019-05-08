#ifndef CSSRULES_H
#define CSSRULES_H

#include "global.h"
#include <map>
#include <string>
#include <vector>

class string_renderer;
class css_node
{
    std::vector<std::string> _selectors;
    std::map<std::string, std::string> _rules;
public:
    css_node();
    void add_rule(const std::string &name, const std::string &value);
    void set_attr(const std::string &name, const std::string &value);
    void add_selector(const std::string &name);
    bool has_selector(const std::string &name);
    std::vector<std::string> selectors() const {
        return _selectors;
    }
    std::string to_string(print_type type = print_type::compact) const;
    void append(string_renderer &r);
    void inline_append(string_renderer &r);

    friend class html_tag;
};

class css_doc : public std::vector<css_node*>
{
public:
    std::string to_string(print_type type = print_type::compact) const;
};

#endif // CSSRULES_H
