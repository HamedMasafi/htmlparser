QT -= gui

CONFIG += c++11 console
CONFIG -= qt


SOURCES += \
        main.cpp \
    $$PWD/htmlparser.cpp \
    $$PWD/htmltag.cpp \
    $$PWD/cssparser.cpp \
    $$PWD/cssrules.cpp \
    $$PWD/tokenparser.cpp \
    string_helper.cpp

HEADERS += \
    $$PWD/htmlparser.h \
    $$PWD/htmltag.h \
    $$PWD/cssparser.h \
    $$PWD/cssrules.h \
    $$PWD/tokenparser.h \
    string_helper.h \
    global.h
