cmake_minimum_required (VERSION 3.0)

project(TestSqliteApp)

include_directories(${JSONCPPLIB_INCLUDE})
include_directories(${DAL_SRC})
include_directories(${SQLREPOLIB_INCLUDE})
include_directories(${SQLITESESSIONLIB_INCLUDE})

file(GLOB DAL_CPP "${DAL_SRC}/*.cpp")
file(GLOB DAL_HEADERS "${DAL_SRC}/*.h")

add_executable(TestSqliteApp main.cpp ${DAL_CPP} ${DAL_HEADERS})
target_link_libraries(TestSqliteApp SqlRepoLib SqliteSessionLib SqliteLib JsonCppLib)
