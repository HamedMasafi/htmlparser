QT -= gui

CONFIG += c++11 console
CONFIG -= qt

INCLUDEPATH += $$PWD/include

SOURCES += main.cpp \
    src/css_document.cpp \
    src/css_node.cpp \
    src/html_document.cpp \
    src/html_node.cpp \
    src/html_tag_vector.cpp \
    src/html_writer.cpp \
    src/query_parser.cpp \
    src/string_helper.cpp \
    src/string_renderer.cpp \
    src/token_parser.cpp \
    src/json_document.cpp \
    src/json_value.cpp \
    src/json_array.cpp \
    src/json_object.cpp

HEADERS += \
    include/css_document.h \
    include/css_node.h \
    include/global.h \
    include/html_document.h \
    include/html_node.h \
    include/html_tag_vector.h \
    include/html_writer.h \
    include/query_parser.h \
    include/string_helper.h \
    include/string_renderer.h \
    include/token_parser.h \
    include/json_document.h \
    include/json_value.h \
    include/json_array.h \
    include/json_object.h
