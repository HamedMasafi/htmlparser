#include "html_parser.h"
#include "html_node.h"
#include "query_parser.h"
#include "string_helper.h"

#include <stack>
#include <vector>
#include <wctype.h>
#include <algorithm>
#include <string_renderer.h>

using namespace std;

html_parser::html_parser() : token_parser(), _root_tag(nullptr)
{
    _literals.push_back(new literal_t{">",  "<",  "",     true,  true});
    _literals.push_back(new literal_t{"\"", "\"", "\\\"", false, true});
    _literals.push_back(new literal_t{"'",  "'",  "\\'",  false, true});

    _check_fns.push_back(&isalpha);
    _check_fns.push_back(&isdigit);
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

    for (; i < _tokens.size(); ++i) {
        string token = _tokens.at(i);

        if (all_of(token.begin(), token.end(),
                   [](char n){return !iswprint(n);}))
            continue;

        if (token == "<") {
            if (_tokens.size() <= i + 1) {
                _error_message = "Unexpected end of document";
                return;
            }

            if (_tokens.at(i + 1) == "/") {
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
        if (token == ">"){
            if (_tokens.size() > i + 1 && _tokens.at(i + 1) != "<") {
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
    if (tag_name == "style")
        tag = new style_tag;
    else
        tag = new html_tag;

    tag->name = tag_name;
//    map<string, string> attrs;
    string name, value;
    int step = 0;
    /*

    name
     =
     "
     value
     "

     */
    if (tokensList.at(i) != ">")
        do {
        if (i == tokensList.size()) {
            _error_message = "Unexpected end of document";
            delete tag;
            return nullptr;
        }
        token = tokensList.at(i++);

        if (token == ">")
            break;

        switch (step) {
        case 0:
            name = token;
            step++;
            break;

        case 1:
            if (token == "=")
                step++;
            else {
                _error_message = "Unexpected " + token + " token";
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
    } while (token != ">");

    tag->setHasCloseTag(tokensList.at(i - 2) != "/");
    i--;
    return tag;
}
