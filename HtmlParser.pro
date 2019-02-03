QT -= gui

CONFIG += c++11 console
CONFIG -= qt


SOURCES += \
        main.cpp \
    html_parser.cpp \
    string_helper.cpp \
    query_parser.cpp \
    html_tag_vector.cpp \
    css_parser.cpp \
    css_doc.cpp \
    html_node.cpp \
    token_parser.cpp

HEADERS += \
    html_parser.h \
    string_helper.h \
    global.h \
    query_parser.h \
    html_tag_vector.h \
    css_parser.h \
    css_doc.h \
    html_node.h \
    token_parser.h
