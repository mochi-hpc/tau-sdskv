include(FindPackageHandleStandardArgs)

find_library(unwind_LIBRARY unwind)
find_path(unwind_INCLUDE_DIR unwind.h)

find_package_handle_standard_args(
    unwind
    FOUND_VAR unwind_FOUND
    REQUIRED_VARS
        unwind_LIBRARY
        unwind_INCLUDE_DIR)

if (unwind_FOUND)
    add_library(unwind::unwind UNKNOWN IMPORTED)
    set_target_properties(
        unwind::unwind
        PROPERTIES
            IMPORTED_LOCATION "${unwind_LIBRARY}"
            IMPORTED_LINK_INTERFACE_LANGUAGES CXX
            INTERFACE_INCLUDE_DIRECTORIES "${unwind_INCLUDE_DIR}"
            INTERFACE_COMPILE_FEATURES cxx_std_14)
endif()