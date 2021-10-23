#include <stdio.h>
#include <omp.h>

int main(){

/*
//Use $(bash) export OMP_NUM_THREADS=X
// X is the number of threads to be used
// X = 50, there will be 50 threads
#pragma omp parallel default(none)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        if(thread_id == 0) printf("There are %d threads\n", total_threads);
    }
*/

//Requesting local number of threads in program
//If used, then it will ignore the request made from bash
#pragma omp parallel num_threads(40) default(none)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        if(thread_id == 0) printf("There are %d threads\n", total_threads);
    }

    return 0;
}

