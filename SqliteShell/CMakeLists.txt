cmake_minimum_required (VERSION 3.0)

project(SqliteShell)

add_executable(SqliteShell ${SQLITELIB_SRC}/shell.c)

if(WIN32)
	target_link_libraries(SqliteShell SqliteLib)
else()
	target_link_libraries(SqliteShell SqliteLib pthread dl)
endif()
