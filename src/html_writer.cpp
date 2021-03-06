#include "html_writer.h"

PARSER_BEGIN_NAMESPACE

html_writer::html_writer()
{
    auto html = new html_tag;
    html->name = "html";
    _tags.push(html);
    _html_tag = html;
}

void html_writer::begin_tag(const std::string &tag_name)
{
    auto tag = new html_tag;
    tag->name = tag_name;
    _tags.push(tag);
}

void html_writer::begin_tag(const html_tag_type &tag_type)
{
    auto tag = new html_tag;
    tag->name = html_tag_str(tag_type);
    _tags.push(tag);
}

void html_writer::write_text(const std::string &text)
{
    auto txt = new text_node;
    txt->set_text(text);
    _tags.top()->add_child(txt);
}

void html_writer::set_attr(const std::string &name, const std::string &value)
{
    if (_tags.size())
        _tags.top()->set_attr(name, value);
}

void html_writer::add_class(const std::string &name)
{
    if (_tags.size())
        _tags.top()->add_class(name);
}

void html_writer::close_tag()
{
    if (_tags.size())
        _tags.pop();
}

std::string html_writer::to_string(const print_type &fmt) const
{
    return _html_tag->to_string(fmt);
}

PARSER_END_NAMESPACE
