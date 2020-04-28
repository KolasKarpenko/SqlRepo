cmake_minimum_required (VERSION 3.0)

project(GenerateDal)

include_directories(${JSONCPPLIB_INCLUDE})
include_directories(${DAL_SRC})
include_directories(${SQLREPOLIB_INCLUDE})
include_directories(${SQLITESESSIONLIB_INCLUDE})

add_executable(GenerateDal main.cpp)

if(WIN32)
	target_link_libraries(GenerateDal SqlRepoLib SqliteSessionLib SqliteLib JsonCppLib)
else()
	target_link_libraries(GenerateDal SqlRepoLib SqliteSessionLib SqliteLib JsonCppLib pthread dl uuid)
endif()
