cmake_minimum_required (VERSION 3.0)

project(SqlRepo)

option(BUILD_POSTGRE_SESSION "Build session for PosgreSQL" OFF)
option(BUILD_MYSQL_SESSION "Build session for MySQL" OFF)


#extlib
add_subdirectory(SqliteLib)
add_subdirectory(SqliteShell)
add_subdirectory(JsonCppLib)
#end extlib

add_subdirectory(SqlRepoLib)
add_subdirectory(SqliteSessionLib)

add_subdirectory(Dal)

if (BUILD_POSTGRE_SESSION)
	add_subdirectory(PostgreSessionLib)
	add_subdirectory(TestPostgresApp)
endif(BUILD_POSTGRE_SESSION)

if (BUILD_MYSQL_SESSION)
	add_subdirectory(MySqlSessionLib)
	add_subdirectory(TestMySqlApp)
endif(BUILD_MYSQL_SESSION)

add_subdirectory(GenerateDal)

add_subdirectory(TestSqliteApp)
