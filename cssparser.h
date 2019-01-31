#ifndef CSSPARSER_H
#define CSSPARSER_H

#include <tokenparser.h>

class css_parser : public token_parser
{
private:
    static int token(wint_t n);
public:
    css_doc doc;
    css_parser();
    void parse();
    void parse_rules(css_node *node, size_t &i);
    virtual ~css_parser();
};

#endif // CSSPARSER_H
