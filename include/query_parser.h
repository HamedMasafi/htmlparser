#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#include "token_parser.h"

#include <string>
#include <vector>

class html_tag;
class query_parser : public token_parser
{
    struct query_rule_t{
        std::wstring id;
        std::wstring tag_name;
        std::vector<std::wstring> classes;
        bool is_child;

        bool is_valid() const;
        bool check(html_tag *tag) const;
    };
    std::vector<std::vector<query_rule_t*>> rules;

public:
    query_parser();

    // token_parser interface
public:
    void parse();
    html_tag *tag;
    std::vector<html_tag*> search();
    void search(std::vector<html_tag*> *tags, html_tag *tag,
                size_t &rule_id,
                std::vector<query_rule_t *> rules);
};

#endif // QUERY_PARSER_H
