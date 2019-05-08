#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "global.h"
#include "html_tag_vector.h"
#include "token_parser.h"

PARSER_BEGIN_NAMESPACE

class html_tag;
class html_document : public token_parser
{
    html_tag *_root_tag;
    std::string doctype;

public:
    html_document();
    virtual ~html_document() override;

    html_tag *root_tag() const;

    html_node *get_by_id(const std::string &id);
    html_tag_vector get_by_tag_name(const std::string &tag_name);
    html_tag_vector get_by_class_name(const std::string &class_name);
    html_tag_vector query(const std::string &q);

    std::string to_string(print_type type = print_type::compact) const;
private:
    void parse() override;
    html_tag *parse_tag_begin(std::vector<std::string> &tokensList, size_t &i);
    static int token(int n);
};

PARSER_END_NAMESPACE

#endif // HTMLPARSER_H
