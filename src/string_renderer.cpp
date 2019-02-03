#include "string_renderer.h"

#define INDENT_SIZE 4
string_renderer::string_renderer(print_type type) : _type(type), _level(0), _last_action(last_action::etc)
{

}

void string_renderer::append(const std::wstring &buffer)
{
    _last_action = last_action::etc;
    _buffer.append(buffer);
}

void string_renderer::append(const std::wstring &&buffer)
{
    _last_action = last_action::etc;
    _buffer.append(buffer);
}

void string_renderer::new_line()
{
    if (_last_action == last_action::new_line)
        return;
    _last_action = last_action::new_line;
    if (_type == print_type::formatted) {
        _buffer.append(L"\n");
        for (unsigned long i = 0; i < INDENT_SIZE * _level; ++i)
            _buffer.append(L" ");
    }
}

void string_renderer::space()
{
    if (_last_action == last_action::space)
        return;
    _last_action = last_action::space;
    if (_type == print_type::formatted)
        _buffer.append(L" ");
}

void string_renderer::indent()
{
    _level++;

    if (_last_action == last_action::new_line)
        for (unsigned long i = 0; i < INDENT_SIZE; ++i)
            _buffer.append(L" ");
}

void string_renderer::unindent()
{
    _level--;

    if (_last_action == last_action::new_line)
        _buffer.erase(_buffer.end() - INDENT_SIZE, _buffer.end());
}

std::wstring string_renderer::to_string() const
{
    return _buffer;
}
