#include <iostream>
#include <omp.h>

int main(){
#pragma omp parallel num_threads(20)
    {
        int thread_id = omp_get_thread_num();
        //The master construct denotes a structured block that is only executed by the master thread(0)
        //The other threads just skip it (no sync is implied), they are not waiting
#pragma omp master
        std::cout << "Master Thread ID: " << thread_id << "\n";
    }
#pragma omp barrier //if we want the other threads to wait for the master
}