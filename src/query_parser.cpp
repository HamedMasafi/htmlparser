#include "html_node.h"
#include "query_parser.h"

#include <wctype.h>
#include <algorithm>
#include <string>
#include <iostream>

query_parser::query_parser() : token_parser()
{
    _check_fns.push_back(&isalnum);
}

void query_parser::parse()
{
    query_rule_t *last_rule = new query_rule_t;

    std::vector<query_rule_t*> rl;
    for (size_t i = 0; i < _tokens.size(); ++i) {
        auto t = _tokens.at(i);
        if (t == L",") {
            rl.push_back(last_rule);
            rules.push_back(rl);
            rl.clear();
            last_rule = new query_rule_t;
            continue;
        }

        if (t == L".") {
            ++i;
            last_rule->classes.push_back(_tokens.at(i));
            continue;
        }
        if (t == L"#") {
            ++i;
            last_rule->id = _tokens.at(i);
            continue;
        }
        if (t == L">") {
            rl.push_back(last_rule);
            last_rule = new query_rule_t;
            last_rule->is_child = true;
            continue;
        }

        if (last_rule->is_valid())
            rl.push_back(last_rule);
        last_rule = new query_rule_t;
        last_rule->is_child = false;
        last_rule->tag_name = t;

    }
    rl.push_back(last_rule);
    rules.push_back(rl);

    int i = 0;
    for (auto l : rules) {
        std::cout << "RULE: " << (++i) << std::endl;
        for (auto r : l) {
            std::wcout << L"   >id:" << r->id
                       << L" tag:" << r->tag_name
                       << L" is child:" << r->is_child
                       << std::endl;
            for (auto cls : r->classes)
                std::wcout << L"      >" << cls << std::endl;
        }
    }
}

std::vector<html_tag *> query_parser::search()
{
    std::vector<html_tag *> tags;
    for (auto l : rules) {
        size_t i = 0;
        search(&tags, tag, i, l);
    }
    return tags;
}

void query_parser::search(std::vector<html_tag *> *tags, html_tag *tag, size_t &rule_id, std::vector<query_rule_t *> rules)
{
    if (rule_id >= rules.size()) {
        return;
    }

    auto rule = rules.at(rule_id);
    if (rule->check(tag)) {

        if (rule_id == rules.size() - 1)
            tags->push_back(tag);
        rule_id++;
    }

    for (auto child : tag->childs()) {
        auto t = dynamic_cast<html_tag*>(child);
        if (t)
            search(tags, t, rule_id, rules);
    }
}

bool query_parser::query_rule_t::is_valid() const {
    return id.size() || tag_name.size() || classes.size();
}

bool query_parser::query_rule_t::check(html_tag *tag) const
{
    if (id.size() && tag->id() != id)
        return false;

    if (tag_name.size() && tag->name != tag_name)
        return false;

    if (classes.size())
        for (auto cls : classes) {
            if (std::none_of(tag->_classes.begin(), tag->_classes.end(), [=](std::wstring c){
                             std::wcout << c << std::endl;
                return c == cls;
            })){
                std::wcout << cls << L" not found" << std::endl;
                return false;
            }
        }

    return true;
}
