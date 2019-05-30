project(tau-sdskeyval-control LANGUAGES CXX)

configure_file(cmake/version.hpp.in generated/version.hpp)

add_library(
    tau_sdskeyval_control
        src/lib.cpp
)
add_library(tau_sdskeyval::control ALIAS tau_sdskeyval_control)

target_include_directories(
    tau_sdskeyval_control
    PUBLIC
        include
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_link_libraries(
    tau_sdskeyval_control
    PRIVATE
        tau_sdskeyval::plugin_internal_control
        dl
)

target_compile_features(
    tau_sdskeyval_control
    PRIVATE
        cxx_std_17
)
