#include "html_parser.h"
#include "html_node.h"
#include "query_parser.h"
#include "string_helper.h"

#include <iostream>
#include <stack>
#include <vector>
#include <wctype.h>
#include <algorithm>

using namespace std;

html_parser::html_parser() : token_parser(), _root_tag(nullptr)
{
    _literals.push_back(new literal_t{L">",  L"<",  L"",     true,  true});
    _literals.push_back(new literal_t{L"\"", L"\"", L"\\\"", false, true});
    _literals.push_back(new literal_t{L"'",  L"'",  L"\\'",  false, true});

    _check_fns.push_back(&iswalpha);
    _check_fns.push_back(&iswdigit);
}

html_parser::~html_parser()
{
    delete _root_tag;
}

html_tag *html_parser::root_tag() const {
    return _root_tag;
}

void html_parser::parse()
{
    size_t i = 0;

    stack<html_tag*> stack;
    vector<html_tag*> tags;
    _root_tag = nullptr;

    if (_tokens.size() > 3 && _tokens.at(0) == L"<" && _tokens.at(1) == L"!") {
        std::wstring token;
        doctype.clear();
        while (token != L">") {
            token = _tokens.at(++i);
            if (token != L">")
                doctype.append(token + L" ");
        }
        ++i;
    }

    for (; i < _tokens.size(); ++i) {
        wstring token = _tokens.at(i);

        if (all_of(token.begin(), token.end(),
                   [](wint_t n){return !iswprint(n);}))
            continue;

        if (token == L"<") {
            if (_tokens.size() <= i + 1) {
                _error_message = L"Unexpected end of document";
                return;
            }

            if (_tokens.at(i + 1) == L"/") {
                stack.pop();
            } else {
                html_tag *tag = parse_tag_begin(_tokens, i);
                if (tag) {
                    if (!tags.size())
                        _root_tag = tag;
                    if (stack.size()) {
                        tag->set_parent(stack.top());
                        stack.top()->add_child(tag);
                    }

                    tags.push_back(tag);
                    if (tag->hasCloseTag())
                        stack.push(tag);
                }
            }
        }

        token = _tokens.at(i);
        if (token == L">"){
            if (_tokens.size() > i + 1 && _tokens.at(i + 1) != L"<") {
                auto text = string_helper::trim_copy(_tokens.at(i + 1));
                if (any_of(text.begin(), text.end(), &iswalpha)) {
                    text_node *textNode = new text_node;
                    textNode->setText(text);
                    stack.top()->add_child(textNode);
                }
            }
        }
    }
}

html_tag *html_parser::get_by_id(const wstring &id)
{
    auto tags = query(L"#" + id);
    if (tags.size())
        return tags.at(0);
    else
        return nullptr;
}

std::vector<html_tag *> html_parser::get_by_tag_name(const wstring &tag_name)
{
    return query(tag_name);
}

std::vector<html_tag *> html_parser::get_by_class_name(const wstring &class_name)
{
    return query(L"." + class_name);
}

std::vector<html_tag *> html_parser::query(const wstring &q)
{
    query_parser qp;
    qp.set_text(q);
    qp.tag = _root_tag;
    return qp.search();
}

wstring html_parser::to_string(print_type type) const
{
    return L"<" + doctype + L">\n" + _root_tag->to_string(type);
}

html_tag *html_parser::parse_tag_begin(std::vector<wstring> &tokensList, size_t &i)
{
    html_tag *tag;
    wstring token;
    i++;
    auto tag_name = tokensList.at(i++);
    if (tag_name == L"style")
        tag = new style_tag;
    else
        tag = new html_tag;

    tag->name = tag_name;
//    map<wstring, wstring> attrs;
    wstring name, value;
    int step = 0;
    /*

    name
     =
     "
     value
     "

     */
    if (tokensList.at(i) != L">")
        do {
        if (i == tokensList.size()) {
            _error_message = L"Unexpected end of document";
            delete tag;
            return nullptr;
        }
        token = tokensList.at(i++);

        if (token == L">")
            break;

        switch (step) {
        case 0:
            name = token;
            step++;
            break;

        case 1:
            if (token == L"=")
                step++;
            else {
                _error_message = L"Unexpected " + token + L" token";
                        //<< "error; token is" << token << "in" << step << "step for" << tag->name;
                delete tag;
                return nullptr;
            }
            break;

        case 2:
            step = 0;
            value = token;
            tag->set_attr(name, value);
            break;
        }
    } while (token != L">");

    tag->setHasCloseTag(tokensList.at(i - 2) != L"/");
    i--;
    return tag;
}
