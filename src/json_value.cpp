#include "json_value.h"
#include "json_object.h"
#include "json_array.h"
#include "string_renderer.h"
#include "string_helper.h"

#include <algorithm>

PARSER_BEGIN_NAMESPACE

json_value::json_value()
{
    _type = json_value_t::invalid;
}

parser::json_value::json_value(const std::string &value) : _s(value)
{
    if (string_helper::is_integer(value)) {
        _n = std::stoi(value);
        _type = json_value_t::int_t;
        return;
    };

    if (string_helper::is_float(value)) {
        _f = std::stof(value);
        _type = json_value_t::float_t;
        return;
    };

    _type = json_value_t::string_t;
}

json_value::~json_value()
{

}

json_value::json_value_t json_value::type() const
{
    return _type;
}

json_array *parser::json_value::to_array()
{
    if (_type == json_value_t::array_t)
        return dynamic_cast<json_array*>(this);
    else
        return nullptr;
}

parser::json_object *parser::json_value::to_object()
{
    if (_type == json_value_t::object_t)
        return dynamic_cast<json_object*>(this);
    else
        return nullptr;
}

std::string parser::json_value::to_string() const
{
    return _s;
}

float parser::json_value::to_float() const
{
    return _f;
}

int parser::json_value::to_int() const
{
    return _n;
}

void parser::json_value::render(string_renderer &r)
{
    switch (_type) {
    case json_value_t::string_t:
        r.append("'");
        r.append(_s);
        r.append("'");
        break;

    case json_value_t::int_t:
    case json_value_t::float_t:
        r.append(_s);
        break;

    default:
        r.append("INVALID");
    }
}

PARSER_END_NAMESPACE
