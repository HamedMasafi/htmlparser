#ifndef CSSPARSER_H
#define CSSPARSER_H

#include "css_doc.h"
#include "token_parser.h"

class css_parser : public token_parser
{
private:
    std::vector<css_node*> _nodes;
    static int token(wint_t n);

public:
    css_parser();
    virtual ~css_parser();

    std::vector<css_node*> find_contains_selector(const std::wstring &selector);
    std::vector<css_node*> find_match_selector(const std::wstring &selector);
    std::map<std::wstring, std::wstring> parse_block();

    std::wstring to_string(print_type type = print_type::compact) const;
    std::vector<css_node *> nodes() const;

private:
    void parse();
};

#endif // CSSPARSER_H
