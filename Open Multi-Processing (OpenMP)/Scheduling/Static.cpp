//Static

#include <iostream>
#include <omp.h>
#include <unistd.h>

int main() {

    // schedule(static,x) it means that to each of the threads will be assigned x contiguous iterations
    // In this case the first thread will take x numbers. The second one will take another x
    // and so on until there are no more data to process or the maximum number of threads is reached
    //If chunk-size variable is not specified, OpenMP will divide iterations into chunks
    // that are approximately equal in size and it distributes chunks to threads in order
    // (That is why static method different from others)
    // in case below threadID: 7 will take i=19 as last iteration
#pragma omp parallel for schedule(static) num_threads(12)
    for(int i = 0; i < 20; i++) {
        sleep(i);
        std::cout << "ThreadID: " << omp_get_thread_num() << " i: " << i << "\n";
    }
    return 0;
}
