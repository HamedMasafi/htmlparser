#include "css_doc.h"
#include "html_node.h"
#include "token_parser.h"
#include "string_helper.h"

#include <wctype.h>
#include <cctype>
#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

token_parser::token_parser()
{
}

token_parser::~token_parser()
{

}

std::string token_parser::text() const
{
    return _text;
}

void token_parser::set_text(const std::string &text)
{
    _text = text;
    parse_tokens();
    parse();
}

std::vector<std::string> token_parser::tokens() const
{
    return _tokens;
}

std::vector<std::string> token_parser::parse_tokens()
{
    _tokens.clear();
    for (std::size_t i = 0; i < _text.length(); ++i) {
        std::string last_token;
        auto ch = static_cast<char>(_text.at(i));

        if (iscntrl(ch) || isspace(ch) || isblank(ch) || ch == '\n' || ch == '\r')
            continue;

        bool outer_continue = false;
        for (literal_t *literal : _literals) {
            string st = _text.substr(i, literal->begin.length());
            if (st == literal->begin) {
                last_token = read_until(_text, i, literal);

                if (literal->insert_content) {
                    if (literal->insert_begin_end)
                        _tokens.push_back(literal->begin);
                    if (is_valid_token(last_token))
                        _tokens.push_back(last_token);

                    if (literal->insert_begin_end)
                        _tokens.push_back(literal->end);
                }

                outer_continue = true;
                break;
            }
        }
        if (outer_continue)
            continue;

        for (auto &fn : _check_fns)
            if (fn(ch)) {
                last_token = read_until(_text, i, fn);
                if (last_token.length()) {
                    _tokens.push_back(last_token);
                    --i;
                    outer_continue = true;
                    break;
                }
            }
        if (outer_continue)
            continue;

        if (ispunct(ch)) {
            _tokens.push_back(_text.substr(i, 1));
        }
    }

    return _tokens;
}

string token_parser::read_until(const string &text, size_t &i, std::function<int (int)> fn) const
{
    size_t start = i;
    while (text.length() > i  && fn(text.at(i))) {
        i++;
    }
    return text.substr(start, i - start);
}

string token_parser::read_until(const string &text, size_t &i, const literal_t *lt) const
{
    auto start = i;
    while (i < text.length() - lt->end.length()) {
        if (text.length() < i + lt->end.length())
            return string();

        auto s = text.substr(++i, lt->end.length());
        if (s == lt->end)
            break;
    }
    if (i == start)
        return string();
    i += lt->end.length() - 1;
    start += lt->begin.length();

    return text.substr(start, i - start - lt->end.length() + 1);
}

bool token_parser::is_valid_token(const string &token) const
{
    if (!token.length())
        return false;

    return any_of(token.begin(), token.end(), [](char ch){
        return isprint(ch);
    });
}

