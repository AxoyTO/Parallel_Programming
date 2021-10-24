#include <iostream>
#include <omp.h>
#include <unistd.h>

int main() {
    /*
     * In such a case we can resort to dynamic scheduling.
     * In dynamic scheduling, each thread will take one iteration,
     * process it, and then see what is the next iteration that is currently not being processed by anyone.
     * This way it will never happen that one thread finishes while other threads have still lots of work to do
     * Similar to the idea of 'nowait'
     */
#pragma omp parallel for schedule(dynamic, 1) num_threads(4) shared(std::cout) default(none)
    for(int i = 0; i < 20; i++) {
        //sleep(i);
        std::cout << "ThreadID: " << omp_get_thread_num() << " i: " << i << "\n";
    }
    return 0;
}