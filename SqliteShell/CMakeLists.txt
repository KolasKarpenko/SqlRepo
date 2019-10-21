cmake_minimum_required (VERSION 3.0)

project(SqliteShell)

add_executable(SqliteShell ${SQLITELIB_SRC}/shell.c)
target_link_libraries(SqliteShell SqliteLib)