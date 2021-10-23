#include <iostream>
#include <omp.h>

int main(){
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        /*
         * The threads go into the critical section one by one, so it's not parallelized
         * Only one thread at a time can enter a critical region.
         */
#pragma omp critical
        std::cout << "ThreadID: " << thread_id;
        std::cout << " is inside of critical section!\n";
    }
    return 0;
}
