#include "css_parser.h"
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
}

void css_parser::parse()
{
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

css_parser::~css_parser()
{

}

std::vector<css_node *> css_parser::find_contains_selector(const std::wstring &selector)
{
    std::vector<css_node *> ret;
    for (css_node *node : doc)
        if (node->has_selector(selector))
            ret.push_back(node);
    return ret;
}

std::vector<css_node *> css_parser::find_match_selector(const std::wstring &selector)
{
    std::vector<css_node *> ret;
    for (css_node *node : doc)
        if (node->selectors().size() == 1 && node->has_selector(selector))
            ret.push_back(node);
    return ret;
}

std::map<std::wstring, std::wstring> css_parser::parse_block()
{
    size_t i = 0;
    std::map<std::wstring, std::wstring> ret;
    while (true) {
        if (_tokens.size() > i + 3)
            break;
        auto name = _tokens.at(i);
        auto colon = _tokens.at(i + 1);
        auto value = _tokens.at(i + 2);

        if (_tokens.size() >= i + 3 && _tokens.at(i + 3) != L";") {
            _error_message = L"Unecpected token: " + _tokens.at(i + 3);
            break;
        }
        ret[name] = value;
    }

    return ret;
}


