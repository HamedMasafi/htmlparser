#ifndef HTML_TAG_VECTOR_H
#define HTML_TAG_VECTOR_H

#include <vector>
#include <string>
#include "global.h"

PARSER_BEGIN_NAMESPACE

class html_node;
class html_tag_vector : public std::vector<html_node*>
{
public:
    html_tag_vector();

    void add_class(const std::string &cls);
    void remove_class(const std::string &cls);
    html_tag_vector find(const std::string &query);

    html_tag_vector tags() const;

    html_node *first_or_null();
    html_node *last_or_null();
};

PARSER_END_NAMESPACE

#endif // HTML_TAG_VECTOR_H
