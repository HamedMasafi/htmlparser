INCLUDEPATH += $$PWD/include

#SOURCES += $$PWD/src/*.cpp
#HEADERS += $$PWD/include/*.h

SOURCES += \
    $$PWD/src/css_document.cpp \
    $$PWD/src/html_document.cpp \
    $$PWD/src/html_node.cpp \
    $$PWD/src/html_tag_vector.cpp \
    $$PWD/src/query_parser.cpp \
    $$PWD/src/string_helper.cpp \
    $$PWD/src/string_renderer.cpp \
    $$PWD/src/token_parser.cpp \
    $$PWD/src/css_node.cpp \
    $$PWD/src/html_writer.cpp

HEADERS += \
    $$PWD/include/css_document.h \
    $$PWD/include/global.h \
    $$PWD/include/html_document.h \
    $$PWD/include/html_node.h \
    $$PWD/include/html_tag_vector.h \
    $$PWD/include/query_parser.h \
    $$PWD/include/string_helper.h \
    $$PWD/include/string_renderer.h \
    $$PWD/include/token_parser.h \
    $$PWD/include/css_node.h \
    $$PWD/include/html_writer.h
