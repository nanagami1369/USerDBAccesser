cmake_minimum_required(VERSION 3.10)
project(UserDBAccesser VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(DB_TYPE "memory" CACHE STRING "ビルド時に使用するデータベースの種類を指定")
message(STATUS "DB_TYPE : ${DB_TYPE}")
message(STATUS "${CMAKE_CXX_FLAGS}")

set(USER_DB_ACCESSER_COMMON_SRC
 ${PROJECT_SOURCE_DIR}/src/User.cpp
 ${PROJECT_SOURCE_DIR}/src/t_Level.cpp
 ${PROJECT_SOURCE_DIR}/src/UserDB.cpp
 ${PROJECT_SOURCE_DIR}/src/PromptModule.cpp
 ${PROJECT_SOURCE_DIR}/src/ValidationException.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Library
if("${DB_TYPE}" MATCHES "^memory$")
    ADD_LIBRARY(UserDB STATIC ${USER_DB_ACCESSER_COMMON_SRC} src/MemoryUserDB.cpp)
    target_link_libraries(UserDB crypt)
elseif("${DB_TYPE}" MATCHES "^sqlite3$")
    add_subdirectory(SQLiteCpp)
    include_directories(SQLiteCpp/include)
    ADD_LIBRARY(UserDB STATIC ${USER_DB_ACCESSER_COMMON_SRC} src/SQLiteUserDB.cpp)
    target_link_libraries(UserDB SQLiteCpp crypt)
endif()

if("${DB_TYPE}" MATCHES "^memory$")
    add_compile_options(-D MEMORY_DB)
elseif("${DB_TYPE}" MATCHES "^sqlite3$")
    add_compile_options(-D SQLITE3_DB)
endif()

# CUI
include_directories(${PROJECT_SOURCE_DIR}/CUI/include)
add_executable(UserDBAccesser CUI/src/main.cpp CUI/src/Controller.cpp)
target_link_libraries(UserDBAccesser UserDB)

#CGI
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)
include_directories(json/include)

include_directories(${PROJECT_SOURCE_DIR}/CGI/include)
add_executable(UserDBAccesser.cgi CGI/src/main.cpp CGI/src/CGIController.cpp)
target_link_libraries(UserDBAccesser.cgi UserDB)

# Test
enable_testing()

add_subdirectory(googletest)
include_directories(googletest/googletest/include)
add_executable(UserTest tests/user_test.cpp)
target_link_libraries(UserTest UserDB gtest_main)
add_test(NAME AllTest COMMAND UserTest)
