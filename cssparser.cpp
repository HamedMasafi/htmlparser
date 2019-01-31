#include "cssparser.h"
#include <wctype.h>

int css_parser::token(wint_t n)
{
    return iswprint(n) && n != '{' && n != '}' && n != ':' && n != ';' && n != ',';
}

css_parser::css_parser()
{
//    _literals.push_back(new literal_t{L"'", L"'", L"\\'", false, false});
    _literals.push_back(new literal_t{L"/*", L"*/", L"", false, false});

    _check_fns.push_back(&css_parser::token);

    _commants.push_back(new comment_t{L"/*", L"*/"});
}

void css_parser::parse()
{
    parse_tokens();
    bool inside_block = false;
    css_node *last_node = new css_node;

    for (size_t i = 0; i < _tokens.size(); ++i) {
        auto token = _tokens.at(i);
        if (token == L"{") {
            inside_block = true;
            continue;
        }
        if (token == L"}") {
            inside_block = false;
            doc.push_back(last_node);
            last_node = new css_node;
            continue;
        }
        if (inside_block) {
            auto colon = _tokens.at(i + 1);
            auto value = _tokens.at(i + 2);
            auto semi_colon = _tokens.at(i + 3);
            last_node->set_attr(token, value);
            i += 3;
            continue;
        } else {
            if (token != L",")
                last_node->add_selector(token);
        }
    }
}

void css_parser::parse_rules(css_node *node, size_t &i)
{
    while (true) {
        if (_tokens.size() < i + 2) {
            _error_message = L"Unexpected end of document";
            return;
        }

        auto name = _tokens.at(i);
        auto colon = _tokens.at(i + 1);
        auto value = _tokens.at(i + 2);

        if (colon != L":") {
            _error_message = L"Unexpected " + colon + L" token, expected ':'";
            return;
        }

        if (_tokens.size() < i + 3) {
            auto semi_colon = _tokens.at(i + 3);
            if (semi_colon != L";" && semi_colon != L"}") {
                _error_message = L"Unexpected " + semi_colon + L" token, expected ';' or '}'";
                return;
            }
            i += 3;
        } else {
            i += 2;
        }

        node->set_attr(name, value);

        if (i == _tokens.size() - 1)
            break;
    }
}

css_parser::~css_parser()
{

}


