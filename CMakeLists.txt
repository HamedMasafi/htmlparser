cmake_minimum_required(VERSION 2.6)
project(html_parser)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

include_directories(include)

add_executable(html_parser ${SOURCES}  main.cpp)
#add_library(libhtmlparser SHARED ${SOURCES})

install(TARGETS html_parser RUNTIME DESTINATION bin)
