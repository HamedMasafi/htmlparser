#ifndef STRING_RENDERER_H
#define STRING_RENDERER_H

#include "global.h"
#include <string>

class string_renderer
{
    std::wstring _buffer;
    print_type _type;
    size_t _level;

    enum class last_action {
        etc,
        new_line,
        space
    };
    last_action _last_action;

public:
    string_renderer(print_type type);
    void append(const std::wstring &buffer);
    void append(const std::wstring &&buffer);
    void new_line();
    void space();
    void indent();
    void unindent();
    std::wstring to_string() const;
};

#endif // STRING_RENDERER_H
