cmake_minimum_required(VERSION 3.26.0)
project(hotreload VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hotreload "")
add_subdirectory(src)