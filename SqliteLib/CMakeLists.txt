cmake_minimum_required (VERSION 3.0)

project(SqliteLib)

add_library(SqliteLib SqliteSrc/sqlite3.c)

set(SQLITELIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/SqliteSrc PARENT_SCOPE)
set(SQLITELIB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/SqliteSrc PARENT_SCOPE)
