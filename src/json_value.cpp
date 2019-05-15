#include "json_value.h"
#include "json_object.h"
#include "json_array.h"
#include "string_renderer.h"
#include "string_helper.h"

#include <algorithm>

PARSER_BEGIN_NAMESPACE

json_value::json_value() : _type(type_t::invalid)
{ }

parser::json_value::json_value(const bool &b) : _b(b), _type(type_t::bool_t)
{ }

parser::json_value::json_value(const int &n) : _n(n), _type(type_t::int_t)
{ }

parser::json_value::json_value(const float &f) : _f(f), _type(type_t::float_t)
{ }

parser::json_value::json_value(const std::string &value) : _s(value), _type(type_t::string_t)
{ }

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

    case type_t::invalid:
        r.append("null");
        break;

    case type_t::bool_t:
        r.append(_b ? "true" : "false");
        break;

    case type_t::int_t:
        r.append(std::to_string(_n));
        break;

    case type_t::float_t:
        r.append(std::to_string(_f));
        break;

    default:
        r.append("INVALID");
    }
}

PARSER_END_NAMESPACE
