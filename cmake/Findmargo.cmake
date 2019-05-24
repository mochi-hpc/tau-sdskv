include(FindPackageHandleStandardArgs)

find_package(mercury REQUIRED)
find_package(argobots REQUIRED)

find_library(margo_LIBRARY margo)
find_path(margo_INCLUDE_DIR margo.h)

find_package_handle_standard_args(
    margo
    FOUND_VAR margo_FOUND
    REQUIRED_VARS
        margo_LIBRARY
        margo_INCLUDE_DIR)

if (margo_FOUND)
    add_library(margo::margo UNKNOWN IMPORTED)
    set_target_properties(
        margo::margo
        PROPERTIES
            IMPORTED_LOCATION "${margo_LIBRARY}"
            IMPORTED_LINK_INTERFACE_LANGUAGES CXX
            INTERFACE_INCLUDE_DIRECTORIES "${margo_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "mercury;argobots::argobots"
            INTERFACE_COMPILE_FEATURES cxx_std_14)
endif()