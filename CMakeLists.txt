cmake_minimum_required (VERSION 3.0)

project(SqlRepo)

#extlib
add_subdirectory(SqliteLib)
add_subdirectory(SqliteShell)
add_subdirectory(JsonCppLib)
#end extlib

add_subdirectory(SqlRepoLib)
add_subdirectory(SqliteSessionLib)
add_subdirectory(PostgreSessionLib)
add_subdirectory(MySqlSessionLib)
add_subdirectory(Dal)
add_subdirectory(GenerateDal)

add_subdirectory(TestSqliteApp)
add_subdirectory(TestPostgresApp)
add_subdirectory(TestMySqlApp)