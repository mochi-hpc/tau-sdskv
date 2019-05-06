include(FindPackageHandleStandardArgs)

find_library(tau_LIBRARY TAU)
find_path(tau_INCLUDE_DIR TAU.h)

find_package_handle_standard_args(
    tau
    FOUND_VAR tau_FOUND
    REQUIRED_VARS
        tau_LIBRARY
        tau_INCLUDE_DIR)

if (tau_FOUND)
    add_library(tau::tau UNKNOWN IMPORTED)
    set_target_properties(
        tau::tau
        PROPERTIES
            IMPORTED_LOCATION "${tau_LIBRARY}"
            IMPORTED_LINK_INTERFACE_LANGUAGES CXX
            INTERFACE_INCLUDE_DIRECTORIES "${tau_INCLUDE_DIR}"
            INTERFACE_COMPILE_DEFINITIONS "TAU_STDCXXLIB;TAU_BFD"
            INTERFACE_COMPILE_FEATURES cxx_std_14)
endif()