#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#include "html_tag_vector.h"
#include "token_parser.h"

#include <string>
#include <vector>

PARSER_BEGIN_NAMESPACE

class html_tag;
class query_parser : public token_parser
{
    struct query_rule_t{
        std::string id;
        std::string tag_name;
        std::vector<std::string> classes;
        std::string attr_name;
        std::string attr_value;
        bool is_child;

        bool is_valid() const;
        bool check(html_tag *tag) const;

        query_rule_t();
    };
    std::vector<std::vector<query_rule_t*>> rules;

public:
    query_parser();

    // token_parser interface
public:
    void parse();
    void parse_attrs(size_t &i, query_rule_t *rule);
    html_tag *tag;
    html_tag_vector search();
    void search(html_tag_vector *tags, html_tag *tag,
                size_t rule_id,
                std::vector<query_rule_t *> rules);
private:
    static int token(int n);
};

PARSER_END_NAMESPACE

#endif // QUERY_PARSER_H
