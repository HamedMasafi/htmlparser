#include "html_parser.h"
#include "html_node.h"
#include "query_parser.h"
#include "string_helper.h"

#include <stack>
#include <vector>
#include <wctype.h>
#include <algorithm>
#include <string_renderer.h>
#include <iostream>

using namespace std;

int html_parser::token(int n)
{
    return isalnum(n) || isdigit(n) || n == '-';
}

html_parser::html_parser() : token_parser(), _root_tag(nullptr)
{
    _literals.push_back(new literal_t{">",  "<",  "",     true,  true});
    _literals.push_back(new literal_t{"\"", "\"", "\\\"", false, true});
    _literals.push_back(new literal_t{"'",  "'",  "\\'",  false, true});
    _literals.push_back(new literal_t{"!--",  "-->",  "",     false,  false});

    _check_fns.push_back(&html_parser::token);
//    _check_fns.push_back(&isdigit);
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

    if (_tokens.size() > 3 && _tokens.at(0) == "<" && _tokens.at(1) == "!") {
        std::string token;
        doctype.clear();
        while (token != ">") {
            token = _tokens.at(++i);
            if (token != ">")
                doctype.append(token + " ");
        }
        ++i;
    }

    std::string dstr;

    for (; i < _tokens.size(); ++i) {
        string token = _tokens.at(i);
        dstr += token;
        if (none_of(token.begin(), token.end(), &iswprint))
//                   [](char n){return isprint(n);}))
            continue;

        if (token == "<") {
            if (_tokens.size() <= i + 1) {
                _error_message = "Unexpected end of document";
                return;
            }

            if (_tokens.at(i + 1) == "/") {
                if (!stack.size()) {
                    std::cerr << "Invalid document: " << _tokens.at(i + 2) << std::endl;
                    return;
                } else {
                    auto tag_expected = _tokens.at(i + 2);
                    auto real_tag = stack.top()->name;
//                    cout << " <=> " << tag_expected << "  ,  " << real_tag << std::endl;
                    stack.pop();
                }
            } else {
                html_tag *tag = parse_tag_begin(_tokens, i);
                if (tag) {
                    if (!_root_tag)
                        _root_tag = tag;

                    if (stack.size()) {
                        tag->set_parent(stack.top());
                        stack.top()->add_child(tag);
                    }

                    tags.push_back(tag);
                    if (tag->hasCloseTag())
                        stack.push(tag);
                    else
//                        std::cout << tag->name << " has not close tag" << endl;
                    continue;
                } else {
                    std::cerr << _error_message << endl;
                }
            }
        }

        token = _tokens.at(i);
        if (token == ">"){
            if (_tokens.size() > i + 1 && _tokens.at(i + 1) != "<") {
                auto text = string_helper::trim_copy(_tokens.at(i + 1));
                if (any_of(text.begin(), text.end(), &iswalpha)) {
                    if (!stack.size()) {
                        std::cerr << "Invalid document" << std::endl << text;
                        return;
                    }

                    text_node *textNode = new text_node;
                    textNode->setText(text);
                    stack.top()->add_child(textNode);
                }
            }
        }
    }
}

html_tag *html_parser::get_by_id(const string &id)
{
    auto tags = query("#" + id);
    if (tags.size())
        return tags.at(0);
    else
        return nullptr;
}

std::vector<html_tag *> html_parser::get_by_tag_name(const string &tag_name)
{
    return query(tag_name);
}

std::vector<html_tag *> html_parser::get_by_class_name(const string &class_name)
{
    return query("." + class_name);
}

std::vector<html_tag *> html_parser::query(const string &q)
{
    query_parser qp;
    qp.set_text(q);
    qp.tag = _root_tag;
    return qp.search();
}

string html_parser::to_string(print_type type) const
{
    string_renderer r(type);

    if (doctype.size()) {
        r.append("<" + doctype + ">");
        r.new_line();
    }
    _root_tag->append(r);
    return r.to_string();
}

html_tag *html_parser::parse_tag_begin(std::vector<string> &tokensList, size_t &i)
{
    html_tag *tag;
    string token;
    i++;
    auto tag_name = tokensList.at(i++);
    if (tag_name=="<")
        return nullptr;

    if (tag_name == "style")
        tag = new style_tag;
    else
        tag = new html_tag;

    tag->name = tag_name;
    string name, value;
    int step = 0;
    /*

     name       0
     =          1
     "          -
     value      2
     "          -

     */
    if (tokensList.at(i) != ">") {
        do {
            if (i == tokensList.size()) {
                _error_message = "Unexpected end of document";
                delete tag;
                return nullptr;
            }
            token = tokensList.at(i++);

            if (token == ">")
                break;

            if (token == "\"")
                continue;

            switch (step) {
            case 0:
                name = token;
                step++;
                break;

            case 1:
                if (token == "=")
                    step++;
                else {
//                    std::cout << "name = " << name << endl;
//                    std::cout << "error:" <<token << " =>" << tag->name<< endl;
                    _error_message = "Unexpected token: " + token + " in parsing " + tag->name;
                    std::cerr << "error; token is " << token << " in " << step << " step for" << tag->name << std::endl;
                    delete tag;
                    return nullptr;
                }
                break;

            case 2:
                step++;
                value = token;
                tag->set_attr(name, value);
                break;
            }
        } while (token != ">");
        i--;
    }

    tag->setHasCloseTag(tokensList.at(i - 1) != "/");

    return tag;
}
