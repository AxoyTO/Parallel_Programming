#include <iostream>
#include <omp.h>

int main() {
    int x = 10;
    /* The data defined outside of a parallel region is shared,
     * which means visible and accessible any thread simultaneously.
     * By default, all variables in the worksharing region are shared
     * except loop iteration counter
     */
    //    #pragma omp parallel num_threads(6) default(none)
    //        {
    //            x++;
    //            std::cout << "Shared x is " << x << "\n";
    //        }

    /*
     * The data defined within a parallel region is private to each thread,
     * which means each thread will have a local copy and use it as a
     * temporary variable. A private variable is not initialized and the
     * value is not maintained for use outside the parallel region.
     * By default, the loop iteration counters in the OpenMP loop constructs are private
     */
#pragma omp parallel num_threads(4)
    {
        int x;
        x = 5;
        std::cout << "Private x is " << x << "\n";
    }
    std::cout << "Shared x is " << x << "\n";

    return 0;
}