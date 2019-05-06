// STL Includes
#include <iostream>
#include <numeric>
#include <vector>

// Third Party Includes
#include <TAU.h>
#include <omp.h>

int main(int argc, char **argv) {
    Tau_init(argc, argv);

    TAU_REGISTER_THREAD();

#pragma omp parallel
    { TAU_REGISTER_THREAD(); }

    {
        TAU_PROFILE("openmp_outer", "void()", TAU_DEFAULT);

#pragma omp parallel
        {
            TAU_PROFILE("openmp_inner", "void()", TAU_DEFAULT);
            int volatile N = 1000000;
            std::vector<int> my_stuff(N);
            std::iota(my_stuff.begin(), my_stuff.end(), 0);
#pragma omp barrier
            std::cout << "Hi from thread " << omp_get_thread_num() << "!" << std::endl;
#pragma omp barrier
        }
    }

    Tau_dump();

    return 0;
}