#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "global.h"
#include "tokenparser.h"

class html_tag;
class html_parser : public token_parser
{
    html_tag *_htmlTag;
    std::wstring doctype;

public:
    html_parser();
    virtual ~html_parser();

    void parse();
    html_tag *root_tag() const {
        return _htmlTag;
    }

    html_tag *get_by_id(const std::wstring &id);
    std::vector<html_tag *> get_by_tag_name(const std::wstring &tag_name);
    std::vector<html_tag *> get_by_class_name(const std::wstring &class_name);

    std::wstring to_string(print_type type = print_type::compact) const;
    std::wstring to_string(html_tag *tag, int level, print_type type = print_type::compact) const;
private:
    html_tag *parse_tag_begin(std::vector<std::wstring> &tokensList, size_t &i);
    void search(std::vector<html_tag *> *tags, html_tag *tag, int &flag,
                            std::function<bool (html_tag *, int &)> callback);

};

#endif // HTMLPARSER_H
