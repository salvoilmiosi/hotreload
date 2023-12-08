cmake_minimum_required(VERSION 3.26.0)
project(hotreload VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)

add_executable(hotreload WIN32 "")
add_subdirectory(src)

target_link_libraries(hotreload PRIVATE shlwapi)
add_dependencies(hotreload shared_object)

target_include_directories(hotreload PRIVATE lib/include)
target_link_options(hotreload PRIVATE -mconsole)