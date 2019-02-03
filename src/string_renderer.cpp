#include "string_renderer.h"

#define INDENT_SIZE 4
string_renderer::string_renderer(print_type type) : _type(type), _level(0)
{

}

void string_renderer::append(const std::wstring &buffer)
{
    _buffer.append(buffer);
}

void string_renderer::append(const std::wstring &&buffer)
{
    _buffer.append(buffer);
}

void string_renderer::new_line()
{
    if (_type == print_type::formatted)
        _buffer.append(L"\n");
        for (ulong i = 0; i < INDENT_SIZE * _level; ++i)
            _buffer.append(L" ");
}

void string_renderer::indent()
{
    _level++;
}

void string_renderer::unindent()
{
    _level--;
}

std::wstring string_renderer::to_string() const
{
    return  _buffer;
}
