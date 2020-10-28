cmake_minimum_required(VERSION 3.1.0)
project(UserDBAccesser VERSION 1.0.0)
add_executable(UserDBAccesser src/main.cpp src/MemoryUserDB.cpp src/Controller.cpp src/User.cpp)