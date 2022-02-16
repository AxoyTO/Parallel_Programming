#include <iostream>
#include <omp.h>

int main(){
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        std::cout << "ThreadID: " << thread_id;
        //The omp barrier directive identifies a synchronization point at which
        // threads in a parallel region will wait until all other threads
        // in that section reach the same point.
        // Statement execution past the omp barrier point then continues in parallel.
#pragma omp barrier
        std::cout << " is in OMP Barrier.\n";

//The omp barrier directive must appear within a block or compound statement. For example:
//if (x!=0) {
//   #pragma omp barrier    /* valid usage    */
//}
//
//if (x!=0)
//   #pragma omp barrier    /* invalid usage  */

    }

    return 0;
}