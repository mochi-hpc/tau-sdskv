cmake_minimum_required(VERSION 3.13)
project(tau-sdskeyval LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(sdskeyval REQUIRED)
find_package(tau REQUIRED)
find_package(OpenMP REQUIRED)

enable_testing()

add_library(
    tausdskvplugin
    SHARED
        src/lib.cpp
        src/plugin.cpp
)

target_include_directories(
    tausdskvplugin
    PUBLIC
        include
    PRIVATE
        internal
)

target_link_libraries(
    tausdskvplugin
    PRIVATE
        tau::tau
        sdskeyval::client
)

target_compile_features(
    tausdskvplugin
    PRIVATE
        cxx_std_17
)

add_subdirectory(example)