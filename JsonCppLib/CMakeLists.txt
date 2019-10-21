cmake_minimum_required (VERSION 3.0)

project(JsonCppLib)

add_library(JsonCppLib JsonCppSrc/jsoncpp.cpp)

set(JSONCPPLIB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/JsonCppSrc PARENT_SCOPE)
