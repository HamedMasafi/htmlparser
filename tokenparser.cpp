#include "cssrules.h"
#include "htmltag.h"
#include "tokenparser.h"
#include "string_helper.h"

#include <wctype.h>
#include <cctype>
#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

std::wstring token_parser::text() const
{
    return _text;
}

void token_parser::setText(const std::wstring &text)
{
    _text = text;
}

token_parser::token_parser()
{
}

token_parser::~token_parser()
{

}

std::vector<std::wstring> token_parser::parse_tokens()
{

    for (std::size_t i = 0; i < _text.length(); ++i) {
        std::wstring last_token;
        auto ch = static_cast<wint_t>(_text.at(i));

        if (iswcntrl(ch) || iswspace(ch) || iswblank(ch) || ch == '\n' || ch == '\r')
            continue;

        bool outer_continue = false;
        for (literal_t *literal : _literals) {
            wstring st = _text.substr(i, literal->begin.length());
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

        if (iswpunct(ch)) {
            _tokens.push_back(_text.substr(i, 1));
        }
    }

    return _tokens;
}

void token_parser::parse()
{
    parse_tokens();
}

wstring token_parser::read_until(const wstring &text, size_t &i, std::function<int (int)> fn) const
{
    size_t start = i;
    while (fn(text.at(i))) {
        i++;
    }
    return text.substr(start, i - start);
}

wstring token_parser::read_until(const wstring &text, size_t &i, const literal_t *lt) const
{
    auto start = i;
    while (i < text.length() - lt->end.length()) {
        if (text.length() < i + lt->end.length())
            return wstring();

        auto s = text.substr(++i, lt->end.length());
        if (s == lt->end)
            break;
    }
    if (i == start)
        return wstring();
    i += lt->end.length() - 1;
    start += lt->begin.length();

    return text.substr(start, i - start - lt->end.length() + 1);
}

bool token_parser::is_valid_token(const wstring &token) const
{
    if (!token.length())
        return false;

    return any_of(token.begin(), token.end(), [](wint_t ch){
        return iswprint(ch);
    });
}

