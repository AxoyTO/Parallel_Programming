#include <iostream>
#include <omp.h>

int main() {

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        // single denotes that the block of code will be executed by only one thread.
        // a barrier is implied at the end of the single block ( can remove the barrier with nowait clause)
        if(thread_id == 5)
#pragma omp single nowait
        {
            std::cout << "ThreadID: " << thread_id;
            std::cout << " is in single.\n";
        }
#pragma omp barrier
        std::cout << thread_id << " ";
    }
    return 0;
}