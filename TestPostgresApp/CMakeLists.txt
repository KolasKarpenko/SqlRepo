cmake_minimum_required (VERSION 3.0)

project(TestPostgresApp)

include_directories(${JSONCPPLIB_INCLUDE})
include_directories(${DAL_SRC})
include_directories(${SQLREPOLIB_INCLUDE})
include_directories(${POSTGRESESSIONLIB_INCLUDE})

file(GLOB DAL_CPP "${DAL_SRC}/*.cpp")
file(GLOB DAL_HEADERS "${DAL_SRC}/*.h")

add_executable(TestPostgresApp main.cpp ${DAL_CPP} ${DAL_HEADERS})

if(WIN32)
	target_link_libraries(TestPostgresApp SqlRepoLib PostgresSessionLib ${PostgreSQL_LIBRARIES} JsonCppLib)
else()
	target_link_libraries(TestPostgresApp SqlRepoLib PostgresSessionLib ${PostgreSQL_LIBRARIES} JsonCppLib pthread dl uuid)
endif()
