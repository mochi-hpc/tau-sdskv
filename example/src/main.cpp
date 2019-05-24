// STL Includes
#include <atomic>
#include <iostream>
#include <numeric>
#include <vector>

// Third Party Includes
#include <TAU.h>
#include <omp.h>

void recurse(int n) {
    TAU_PROFILE("recurse", "void(int)", TAU_DEFAULT);

    if (n > 0) {
        recurse(n - 1);
    }
}

int main(int argc, char **argv) {
    Tau_init(argc, argv);

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
        }

#pragma omp parallel for
        for (auto i = 0; i < 1000000; i++) {
            TAU_PROFILE("opemp_loop", "void(int)", TAU_DEFAULT);
        }

#pragma omp parallel
        { recurse(1000); }

#pragma omp parallel
        std::cout << "Hi from thread " << omp_get_thread_num() << "!" << std::endl;
    }

#pragma omp parallel
    Tau_dump();

    return 0;
}