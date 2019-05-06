include(FindPackageHandleStandardArgs)

find_library(sdskeyval_client_LIBRARY sdskv-client)
find_library(sdskeyval_server_LIBRARY sdskv-server)
find_path(sdskeyval_INCLUDE_DIR sdskv-client.h)

find_package_handle_standard_args(
    sdskeyval
    FOUND_VAR sdskeyval_FOUND
    REQUIRED_VARS
        sdskeyval_client_LIBRARY
        sdskeyval_server_LIBRARY
        sdskeyval_INCLUDE_DIR)

if (sdskeyval_FOUND)
    add_library(sdskeyval::client UNKNOWN IMPORTED)
    set_target_properties(
        sdskeyval::client
        PROPERTIES
            IMPORTED_LOCATION "${sdskeyval_client_LIBRARY}"
            IMPORTED_LINK_INTERFACE_LANGUAGES CXX
            INTERFACE_INCLUDE_DIRECTORIES "${sdskeyval_INCLUDE_DIR}"
            INTERFACE_COMPILE_FEATURES cxx_std_14)

    add_library(sdskeyval::server UNKNOWN IMPORTED)
    set_target_properties(
        sdskeyval::server
        PROPERTIES
            IMPORTED_LOCATION "${sdskeyval_server_LIBRARY}"
            IMPORTED_LINK_INTERFACE_LANGUAGES CXX
            INTERFACE_INCLUDE_DIRECTORIES "${sdskeyval_INCLUDE_DIR}"
            INTERFACE_COMPILE_FEATURES cxx_std_14)
endif()