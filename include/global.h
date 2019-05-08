#ifndef GLOBAL_H
#define GLOBAL_H

enum class print_type {
    compact,
    formatted
};

#define PARSER_NAMESPACE parser

#ifdef PARSER_NAMESPACE
#   define PARSER_BEGIN_NAMESPACE namespace parser {
#   define PARSER_END_NAMESPACE }
#else
#   define PARSER_BEGIN_NAMESPACE
#   define PARSER_END_NAMESPACE
#endif

#endif // GLOBAL_H
