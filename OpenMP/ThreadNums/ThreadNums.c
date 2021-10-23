//Use $(bash) export OMP_NUM_THREADS=X
// X is the number of threads to be used
// X = 50, there will be 50 threads

#include <stdio.h>
#include <omp.h>

int main(){

#pragma omp parallel default(none)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        if(thread_id == 0) printf("There are %d threads\n", total_threads);
    }
    return 0;
}
