cmake_minimum_required(VERSION 3.10)
project(UserDBAccesser VERSION 1.0.0)

set(DB_TYPE "memory" CACHE STRING "ビルド時に使用するデータベースの種類を指定")
message(STATUS "DB_TYPE : ${DB_TYPE}")

set(USER_DB_ACCESSER_COMMON_SRC
 ${PROJECT_SOURCE_DIR}/src/User.cpp
 ${PROJECT_SOURCE_DIR}/src/t_Level.cpp
 ${PROJECT_SOURCE_DIR}/src/getHashPassWord.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Library
if("${DB_TYPE}" MATCHES "^memory$")
    ADD_LIBRARY(UserDB SHARED ${USER_DB_ACCESSER_COMMON_SRC} src/MemoryUserDB.cpp)
    target_link_libraries(UserDB crypt)
elseif("${DB_TYPE}" MATCHES "^sqlite3$")
    add_subdirectory(SQLiteCpp)
    include_directories(SQLiteCpp/include)
    ADD_LIBRARY(UserDB SHARED ${USER_DB_ACCESSER_COMMON_SRC} src/SQLiteUserDB.cpp)
    target_link_libraries(UserDB SQLiteCpp crypt)
endif()


# CLI
add_executable(UserDBAccesser src/main.cpp src/Controller.cpp)
target_link_libraries(UserDBAccesser UserDB)


# Test
enable_testing()

add_subdirectory(googletest)
include_directories(googletest/googletest/include)
add_executable(UserTest tests/user_test.cpp)
target_link_libraries(UserTest UserDB gtest_main)
add_test(NAME AllTest COMMAND UserTest)
