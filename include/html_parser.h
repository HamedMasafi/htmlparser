#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "global.h"
#include "token_parser.h"

class html_tag;
class html_parser : public token_parser
{
    html_tag *_root_tag;
    std::wstring doctype;

public:
    html_parser();
    virtual ~html_parser();

    html_tag *root_tag() const;

    html_tag *get_by_id(const std::wstring &id);
    std::vector<html_tag *> get_by_tag_name(const std::wstring &tag_name);
    std::vector<html_tag *> get_by_class_name(const std::wstring &class_name);
    std::vector<html_tag *> query(const std::wstring &q);

    std::wstring to_string(print_type type = print_type::compact) const;
private:
    void parse() override;
    html_tag *parse_tag_begin(std::vector<std::wstring> &tokensList, size_t &i);
};

#endif // HTMLPARSER_H
