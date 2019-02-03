#ifndef HTML_TAG_VECTOR_H
#define HTML_TAG_VECTOR_H

#include <vector>

class html_parser;
class html_tag_vector : public std::vector<html_parser*>
{
public:
    html_tag_vector();
};

#endif // HTML_TAG_VECTOR_H
