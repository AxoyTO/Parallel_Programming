#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

#include <iostream>
#include <omp.h>

int main(){
    auto *x = new int[20];
    auto *p = x;
    auto tmp = new int[20];
    omp_set_num_threads(20);
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        /*
         * An atomic operation applies only to the single assignment statement that
         * immediately follows it. It is commonly used to update counters and other simple variables
         * that are accessed by multiple threads simultaneously
         */
        for(int i = 0; i < 10; i++){
            /* Protect against race conditions among multiple updates. */
#pragma omp atomic
            x[thread_id] += thread_id + 1;
            // 10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 160 170 180 190 200

            /* Protect against race conditions with updates through x. */
#pragma omp atomic
            p[thread_id] += thread_id + 1;
            // 20 40 60 80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400

            // Reads the value of a variable atomically.
            // The value of a shared variable can be read safely, avoiding the danger of reading
            // an intermediate value of the variable when it is accessed simultaneously by a concurrent thread.
#pragma omp atomic read
            tmp[thread_id] = x[thread_id];

            // Writes the value of a variable atomically.
            // The value of a shared variable can be written exclusively to avoid errors from simultaneous writes.
#pragma omp atomic write
            x[thread_id] = 0;
            // x -> 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
            // tmp -> 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40

            // Updates the value of a variable atomically.
            // Guarantees that only one thread at a time updates the shared variable,
            // avoiding errors from simultaneous writes to the same variable.
            // An omp atomic directive without a clause is equivalent to an omp atomic update.
#pragma omp atomic update
            x[thread_id] += 1;
            // x -> 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2
            // tmp -> 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41

        }
    }
    for(int i = 0; i < 20; i++){
        std::cout << x[i] << " ";
    }
    std::cout << "\n";
    for(int i = 0; i < 20; i++){
        std::cout << tmp[i] << " ";
    }

    return 0;
}