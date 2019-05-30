project(tau-sdskeyval-plugin LANGUAGES CXX)

# Header file for control of the plugin
add_library(
    plugin_internal_control
    INTERFACE
)
add_library(tau_sdskeyval::plugin_internal_control ALIAS plugin_internal_control)

target_include_directories(
    plugin_internal_control
    INTERFACE
        include
)

# Implements plugin
configure_file(cmake/version.cpp.in version.cpp)

add_library(
    tau_sdskeyval_plugin
    SHARED
        src/lib.cpp
        src/margo.cpp
        src/mercury.cpp
        src/plugin.cpp
        src/sdskv_client.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)
add_library(tau_sdskeyval::plugin ALIAS tau_sdskeyval_plugin)

target_include_directories(
    tau_sdskeyval_plugin
    PRIVATE
        internal
)

target_link_libraries(
    tau_sdskeyval_plugin
    PRIVATE
        tau_sdskeyval::plugin_internal_control
        tau::tau
        sdskeyval::client
)

target_compile_features(
    tau_sdskeyval_plugin
    PRIVATE
        cxx_std_17
)
