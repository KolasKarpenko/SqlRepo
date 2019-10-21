cmake_minimum_required (VERSION 3.0)

project(TestMySqlApp)

include_directories(${JSONCPPLIB_INCLUDE})
include_directories(${DAL_SRC})
include_directories(${SQLREPOLIB_INCLUDE})
include_directories(${MYSQLSESSIONLIB_INCLUDE})

file(GLOB DAL_CPP "${DAL_SRC}/*.cpp")
file(GLOB DAL_HEADERS "${DAL_SRC}/*.h")

add_executable(TestMySqlApp main.cpp ${DAL_CPP} ${DAL_HEADERS})
target_link_libraries(TestMySqlApp SqlRepoLib MySqlSessionLib ${MySQL_LIBRARIES} JsonCppLib)
