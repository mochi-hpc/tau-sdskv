include(FindPackageHandleStandardArgs)

find_library(argobots_LIBRARY abt)
find_path(argobots_INCLUDE_DIR abt.h)

find_package_handle_standard_args(
    argobots
    FOUND_VAR argobots_FOUND
    REQUIRED_VARS
        argobots_LIBRARY
        argobots_INCLUDE_DIR)

if (argobots_FOUND)
    add_library(argobots::argobots UNKNOWN IMPORTED)
    set_target_properties(
        argobots::argobots
        PROPERTIES
            IMPORTED_LOCATION "${argobots_LIBRARY}"
            IMPORTED_LINK_INTERFACE_LANGUAGES CXX
            INTERFACE_INCLUDE_DIRECTORIES "${argobots_INCLUDE_DIR}"
            INTERFACE_COMPILE_FEATURES cxx_std_14)
endif()