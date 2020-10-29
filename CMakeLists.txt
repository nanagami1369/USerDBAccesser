cmake_minimum_required(VERSION 3.1.0)
project(UserDBAccesser VERSION 1.0.0)

set(DB_TYPE "memory" CACHE STRING "ビルド時に使用するデータベースの種類を指定")

message(STATUS "DB_TYPE : ${DB_TYPE}")

if("${DB_TYPE}" MATCHES "^memory$")
    add_executable(UserDBAccesser src/main.cpp src/MemoryUserDB.cpp src/Controller.cpp src/User.cpp src/t_Level.cpp)
    target_link_libraries(UserDBAccesser crypt)
elseif("${DB_TYPE}" MATCHES "^sqlite3$")
    add_subdirectory(SQLiteCpp)
    include_directories(SQLiteCpp/include)
    add_executable(UserDBAccesser src/main.cpp src/SQLiteUserDB.cpp src/Controller.cpp src/User.cpp src/t_Level.cpp)
target_link_libraries(UserDBAccesser SQLiteCpp crypt)
endif()
