#ifndef CSSPARSER_H
#define CSSPARSER_H

#include "css_node.h"
#include "token_parser.h"

PARSER_BEGIN_NAMESPACE

class css_document : public token_parser
{
private:
    std::vector<css_node*> _nodes;
    static int token(int n);

public:
    css_document();
    virtual ~css_document();

    std::vector<css_node*> find_contains_selector(const std::string &selector);
    std::vector<css_node*> find_match_selector(const std::string &selector);
    std::map<std::string, std::string> parse_block();

    std::string to_string(print_type type = print_type::compact) const;
    std::vector<css_node *> nodes() const;

private:
    void parse();
};

PARSER_END_NAMESPACE

#endif // CSSPARSER_H
