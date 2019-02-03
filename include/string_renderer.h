#ifndef STRING_RENDERER_H
#define STRING_RENDERER_H

#include "global.h"
#include <string>

class string_renderer
{
    std::wstring _buffer;
    print_type _type;
    size_t _level;

public:
    string_renderer(print_type type);
    void append(const std::wstring &buffer);
    void append(const std::wstring &&buffer);
    void new_line();
    void indent();
    void unindent();
};

#endif // STRING_RENDERER_H
