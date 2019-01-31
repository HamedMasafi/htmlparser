#include "htmlparser.h"
#include "htmltag.h"
#include "string_helper.h"

#include <stack>
#include <vector>
#include <wctype.h>
#include <algorithm>

using namespace std;

html_parser::html_parser() : token_parser()
{
    _literals.push_back(new literal_t{L">",  L"<",  L"",     true,  true});
    _literals.push_back(new literal_t{L"\"", L"\"", L"\\\"", false, true});
    _literals.push_back(new literal_t{L"'",  L"'",  L"\\'",  false, true});

    _check_fns.push_back(&iswalpha);
    _check_fns.push_back(&iswdigit);

    ignores.push_back('\r');
    ignores.push_back('\n');
}

html_parser::~html_parser()
{

}

void html_parser::parse()
{
    parse_tokens();
    size_t i = 0;

    stack<html_tag*> stack;
    vector<html_tag*> tags;
    _htmlTag = nullptr;

    if (_tokens.size() > 3 && _tokens.at(0) == L"<" && _tokens.at(1) == L"!") {
        std::wstring token;
        doctype.clear();
        i++;
        while (token != L">") {
            token = _tokens.at(++i);
            if (token == L">")
                break;
            if (doctype.size())
                doctype.append(L" ");
            doctype.append(token);
        }
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
                        _htmlTag = tag;
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
                text_node *textNode = new text_node;
                textNode->setText(string_helper::trim_copy(_tokens.at(i + 1)));
                stack.top()->add_child(textNode);
            }
        }
    }
}

html_tag *html_parser::get_by_id(const wstring &id)
{
    vector<html_tag*> ret;
    int f = 0;
    search(&ret, _htmlTag, f, [=](html_tag *t, int &) {
       return t->id() == id;
    });
    return ret.size() ? ret.at(0) : nullptr;

}

std::vector<html_tag *> html_parser::get_by_tag_name(const wstring &tag_name)
{
    vector<html_tag*> ret;
    int f = 0;
    search(&ret, _htmlTag, f, [=](html_tag *t, int &) {
       return t->name == tag_name;
    });
    return ret;
}

std::vector<html_tag *> html_parser::get_by_class_name(const wstring &class_name)
{
    vector<html_tag*> ret;
    int f = 0;
    search(&ret, _htmlTag, f, [=](html_tag *t, int &) {
       return t->has_class(class_name);
    });
    return ret;
}

wstring html_parser::to_string(print_type type) const
{
    return L"<!" + doctype + L">\n" + _htmlTag->outter_html(type);
}

wstring html_parser::to_string(html_tag *tag, int level, print_type type) const
{
    return L"";
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

void html_parser::search(std::vector<html_tag *> *tags, html_tag *tag, int &flag, std::function<bool (html_tag *, int &)> callback)
{
    if (callback(tag, flag))
        tags->push_back(tag);
    for (html_node *node : tag->childs()) {
        html_tag *t = dynamic_cast<html_tag*>(node);
        if (t)
            search(tags, t, flag, callback);
    }
}
