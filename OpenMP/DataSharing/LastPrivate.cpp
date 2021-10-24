#include <iostream>
#include <omp.h>

int main() {
    int x = 10;
/* lastprivate(x) is used with for!
 * lastprivate(x) will copy the variable by the memory.
 * Any updates on the variable inside the worksharing space will update
 * the variable outside of it too.
 * So using it is a little risky.
 */
#pragma omp parallel for lastprivate(x)
    for(int i = 0; i < 5; i++) {
        x = i;
        std::cout << "[INSIDE] x: " << x << "\n";
    }
    std::cout << "[OUTSIDE] x: " << x << "\n";

    return 0;
}