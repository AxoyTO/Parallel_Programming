#include <iostream>
#include <omp.h>

int main() {
    int x = 10;
    /* firstprivate(x) is basically the same as private(x).
     * But firstprivate(x) won't remove the initial value of x.
     * firstprivate(x) will just copy the variable with it's value.
     */
#pragma omp parallel firstprivate(x)
    {
        //x = 5;
        std::cout << "[INSIDE] x: " << x << "\n";
    }
    std::cout << "[OUTSIDE] x: " << x << "\n";

    return 0;
}