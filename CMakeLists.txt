cmake_minimum_required(VERSION 3.13)
project(
    tau-sdskeyval
    VERSION 0.1
    LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(sdskeyval REQUIRED)
find_package(tau REQUIRED)
find_package(OpenMP REQUIRED)

enable_testing()

add_subdirectory(plugin)
add_subdirectory(control)
add_subdirectory(example)