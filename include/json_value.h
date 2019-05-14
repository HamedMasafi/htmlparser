#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include "global.h"
#include <string>

PARSER_BEGIN_NAMESPACE

class json_array;
class json_object;
class string_renderer;
class json_value
{
    int _n;
    float _f;
    std::string _s;

public:
    enum class type_t{
        invalid,
        int_t,
        float_t,
        string_t,
        object_t,
        array_t
    };
    json_value();
    json_value(const std::string &value);
    virtual ~json_value();

    type_t type() const;

    int to_int() const;
    float to_float() const;
    std::string to_string() const;
    json_object *to_object();
    json_array *to_array();

//    template<class T>
//    void *to();

//    template<class T>
//    int to<int>();

protected:
    type_t _type;

private:
    virtual void render(string_renderer &r);

    friend class json_object;
    friend class json_array;
    friend class json_document;
};

PARSER_END_NAMESPACE

#endif // JSON_VALUE_H
