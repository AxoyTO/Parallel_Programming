//Parallel for loop

#include <iostream>
#include <omp.h>

int main() {
    /*
#pragma omp parallel for num_threads(5)
    for(int i = 0; i < 2 * omp_get_num_threads(); i++) {
        std::cout << "ThreadID: " << omp_get_thread_num() << " i: " << i << "\n";
    }
    */

    //above and below are the same
#pragma omp parallel num_threads(5)
    {
#pragma omp for
        for(int i = 0; i < 2 * omp_get_num_threads(); i++) {
            std::cout << "ThreadID: " << omp_get_thread_num() << " i: " << i << "\n";
        }
    }

    return 0;
}