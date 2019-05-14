#include "json_value.h"
#include "json_object.h"
#include "json_array.h"
#include "string_renderer.h"
#include "string_helper.h"

#include <algorithm>

PARSER_BEGIN_NAMESPACE

json_value::json_value() : _type(type_t::invalid)
{ }

parser::json_value::json_value(const std::string &value) : _s(value)
{
    if (value == "true") {
        _type = type_t::bool_t;
        _b = true;
        return;
    }

    if (value == "false") {
        _type = type_t::bool_t;
        _b = false;
        return;
    }

    if (string_helper::is_integer(value)) {
        _n = std::stoi(value);
        _type = type_t::int_t;
        return;
    };

    if (string_helper::is_float(value)) {
        _f = std::stof(value);
        _type = type_t::float_t;
        return;
    };

    _type = type_t::string_t;
}

json_value::~json_value()
{

}

json_value::type_t json_value::type() const
{
    return _type;
}

json_array *parser::json_value::to_array()
{
    if (_type == type_t::array_t)
        return dynamic_cast<json_array*>(this);
    else
        return nullptr;
}

parser::json_object *parser::json_value::to_object()
{
    if (_type == type_t::object_t)
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

bool parser::json_value::to_bool() const
{
    return _b;
}

int parser::json_value::to_int() const
{
    return _n;
}

void parser::json_value::render(string_renderer &r)
{
    auto val = _s;
    bool single_cotation = false;
    bool double_cotation = false;
    std::for_each(val.begin(), val.end(), [&](int ch){
        if (ch == '\'')
            single_cotation = true;
        if (ch == '"')
            double_cotation = true;
    });

    if (single_cotation && double_cotation)
        string_helper::replace(val, "\"", "\\\"");

    switch (_type) {
    case type_t::string_t:
        r.append(single_cotation ? "\"" : "'");
        r.append(val);
        r.append(single_cotation ? "\"" : "'");
        break;

    case type_t::bool_t:
    case type_t::int_t:
    case type_t::float_t:
        r.append(_s);
        break;

    default:
        r.append("INVALID");
    }
}

PARSER_END_NAMESPACE
