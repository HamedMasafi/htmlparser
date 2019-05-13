#ifndef JSON_DOCUMENT_H
#define JSON_DOCUMENT_H

#include "token_parser.h"

PARSER_BEGIN_NAMESPACE

class json_object;
class json_array;
class json_value;
class json_document : public token_parser
{
    static int token(int n);
    json_object *_object;
    json_array *_array;

public:
    json_document();
    std::string to_string(print_type type = print_type::compact) const;
    json_value *find(const std::string &path);

protected:
    void parse();

private:
    json_value *parse_value();
    json_value *parse_value(const std::string &token);
    json_object *parse_object();
    json_array *parse_array();
};

PARSER_END_NAMESPACE

#endif // JSON_DOCUMENT_H
