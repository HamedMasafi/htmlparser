#ifndef TOKENPARSER_H
#define TOKENPARSER_H

#include "cssrules.h"

#include <functional>
#include <vector>
#include <list>

class token_parser
{
protected:
    struct comment_t {
        std::wstring begin;
        std::wstring end;
    };

    struct literal_t {
        std::wstring begin;
        std::wstring end;
        std::wstring ignore;
        bool insert_begin_end;
        bool insert_content;
    };

    std::wstring _error_message;

    std::vector<comment_t*> _commants;
    std::vector<literal_t*> _literals;

    std::vector<std::wstring> _char_types;
    std::vector<wint_t> ignores;

    std::vector<std::function<int (wint_t)>> _check_fns;

    std::wstring _text;
public:
    std::vector<std::wstring> _tokens;
    token_parser();
    virtual ~token_parser();
    std::vector<std::wstring> parse_tokens();
    virtual void parse();

    std::wstring text() const;
    void setText(const std::wstring &text);

protected:
    std::wstring read_until(const std::wstring &text, size_t &i, std::function<int(int)> fn) const;
    std::wstring read_until(const std::wstring &text, size_t &i, const literal_t *lt) const;
    bool is_valid_token(const std::wstring &token) const;
};





#endif // TOKENPARSER_H
