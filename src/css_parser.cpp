#include "css_parser.h"
#include "string_renderer.h"
#include <wctype.h>
#include <iostream>

std::vector<css_node *> css_parser::nodes() const
{
    return _nodes;
}

int css_parser::token(int n)
{
    return isprint(n) && n != '{' && n != '}' && n != ':' && n != ';' && n != ',';
}

css_parser::css_parser()
{
//    _literals.push_back(new literal_t{"'", "'", "\\'", false, false});
    _literals.push_back(new literal_t{"/*", "*/", "", false, false});

    _check_fns.push_back(&css_parser::token);
}

void css_parser::parse()
{
    bool inside_block = false;
    css_node *last_node = new css_node;

    for (size_t i = 0; i < _tokens.size(); ++i) {
        auto token = _tokens.at(i);
        if (token == "{") {
            inside_block = true;
            continue;
        }
        if (token == "}") {
            inside_block = false;
            _nodes.push_back(last_node);
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
            if (token != ",")
                last_node->add_selector(token);
        }
    }
}

css_parser::~css_parser()
{

}

std::vector<css_node *> css_parser::find_contains_selector(const std::string &selector)
{
    std::vector<css_node *> ret;
    for (css_node *node : _nodes)
        if (node->has_selector(selector))
            ret.push_back(node);
    return ret;
}

std::vector<css_node *> css_parser::find_match_selector(const std::string &selector)
{
    std::vector<css_node *> ret;
    for (css_node *node : _nodes)
        if (node->selectors().size() == 1 && node->has_selector(selector))
            ret.push_back(node);
    return ret;
}

std::map<std::string, std::string> css_parser::parse_block()
{
    size_t i = 0;
    std::map<std::string, std::string> ret;
    std::cout << "css count:" << _tokens.size() << std::endl;
    while (true) {
        if (_tokens.size() < i + 3)
            break;
        auto name = _tokens.at(i);
        auto colon = _tokens.at(i + 1);
        auto value = _tokens.at(i + 2);

        std::cout << std::endl << name << colon << value << std::endl;
        if (_tokens.size() >= i + 3 && _tokens.at(i + 3) != ";") {
            _error_message = "Unecpected token: " + _tokens.at(i + 3);
            break;
        }
        ret[name] = value;
        i += 4;
    }

    return ret;
}

std::string css_parser::to_string(print_type type) const
{
    string_renderer r(type);
    for (auto i = _nodes.cbegin(); i != _nodes.cend(); ++i)
        (*i)->append(r);
    return r.to_string();
}


