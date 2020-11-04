cmake_minimum_required(VERSION 3.1.0)
project(UserDBAccesser VERSION 1.0.0)

set(DB_TYPE "memory" CACHE STRING "ビルド時に使用するデータベースの種類を指定")

message(STATUS "DB_TYPE : ${DB_TYPE}")

set(USER_DB_ACCESSER_COMMON_SRC
 ${PROJECT_SOURCE_DIR}/src/main.cpp
 ${PROJECT_SOURCE_DIR}/src/Controller.cpp
 ${PROJECT_SOURCE_DIR}/src/User.cpp
 ${PROJECT_SOURCE_DIR}/src/t_Level.cpp
 ${PROJECT_SOURCE_DIR}/src/getHashPassWord.cpp
)

if("${DB_TYPE}" MATCHES "^memory$")
    add_executable(UserDBAccesser ${USER_DB_ACCESSER_COMMON_SRC} src/MemoryUserDB.cpp)
    target_link_libraries(UserDBAccesser crypt)
elseif("${DB_TYPE}" MATCHES "^sqlite3$")
    add_subdirectory(SQLiteCpp)
    include_directories(SQLiteCpp/include)
    add_executable(UserDBAccesser ${USER_DB_ACCESSER_SQLITE3_SRC} src/SQLiteUserDB.cpp)
    target_link_libraries(UserDBAccesser SQLiteCpp crypt)
endif()
