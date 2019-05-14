#include <algorithm>

#include "json_array.h"
#include "json_value.h"
#include "string_renderer.h"

PARSER_BEGIN_NAMESPACE

json_array::json_array() : json_value()
{

}

parser::json_array::~json_array()
{

}

void parser::json_array::add(parser::json_value *v)
{
    _values.push_back(v);
}

parser::json_value *parser::json_array::at(const size_t &pos) const
{
    return _values.at(pos);
}

void json_array::render(string_renderer &r)
{
    if (!_values.size()) {
        r.append("[]");
        return;
    }

    bool is_simple = std::all_of(_values.begin(), _values.end(), [](json_value *v){
            auto t = v->type();
            return t == type_t::int_t || t == type_t::float_t || t == type_t::string_t;
    });
    r.append("[");
    if (!is_simple) {
        r.new_line();
        r.indent();
    }
    auto count = _values.size();
    for (auto i = _values.begin(); i != _values.end(); ++i) {
        (*i)->render(r);

        if (--count) {
            r.append(",");
            r.space();
        }

        if (!is_simple)
            r.new_line();
    }
    if (!is_simple) {
        r.new_line();
        r.unindent();
    }
    r.append("]");
}

PARSER_END_NAMESPACE
