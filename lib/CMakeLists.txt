cmake_minimum_required(VERSION 3.26.0)
project(hotreload VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(shared_object SHARED)
target_include_directories(shared_object PUBLIC include)

find_package(fmt REQUIRED)
target_link_libraries(shared_object PRIVATE fmt::fmt-header-only)

add_subdirectory(src)

include(GenerateExportHeader)
generate_export_header(shared_object
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/exports/shared_object_export.h"
)

target_include_directories(shared_object PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/exports")