#include <iostream>
#include <omp.h>

int main() {
    int x = 10;
    /* private(x) copies the variable x for each thread separately
     * so they will have their own value which won't interfere
     * with the real x value.
     * private(x) removes the initial value of x and awaits
     * initialization of the variable with a new value for worksharing region.
     */
#pragma omp parallel private(x)
    {
        if(omp_get_thread_num() == 1)
            x = 1;
        if(omp_get_thread_num() == 2)
            x = 2;
        if(omp_get_thread_num() == 9)
            x = 9;
        std::cout << "[INSIDE] x: " << x << "\n";
    }
    std::cout << "[OUTSIDE] x: " << x << "\n";
    return 0;
}