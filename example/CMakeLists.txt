project(tau-sdskeyval-example LANGUAGES CXX)

# find_package(MPI 3.0 REQUIRED)

add_executable(
    tautest
        src/main.cpp
)

target_link_libraries(
    tautest
    PRIVATE
        tau::tau
        OpenMP::OpenMP_CXX
        # MPI::MPI_CXX
)

target_compile_definitions(
    tautest
    PRIVATE
        PROFILING_ON
)

target_compile_features(
    tautest
    PRIVATE
        cxx_std_17
)

find_program(TAUEXEC tau_exec)

add_test(
    NAME tausdskv-test
    COMMAND ${TAUEXEC} -T serial,openmp -ompt "$<TARGET_FILE:tautest>")
set_tests_properties(
    tausdskv-test
    PROPERTIES
        ENVIRONMENT "TAU_PLUGINS_PATH=;TAU_PLUGINS=$<TARGET_FILE:tausdskvplugin>"
)