//Parallel for loop

#include <iostream>
#include <omp.h>

int main() {
    //collapse(n): Allows to parallelize multiple loops in a nest without introducing nested parallelism.
    //Only one collapse clause is allowed on a worksharing for or parallel for pragma.
    //The specified number of loops must be present lexically. That is, none of the loops can be in a called subroutine.
    //The loops must form a rectangular iteration space and the bounds and stride of each loop must be invariant over all the loops.
    //If the loop indices are of different size, the index with the largest size will be used for the collapsed loop.
    //The loops must be perfectly nested; that is, there is no intervening code nor any OpenMP pragma between the loops which are collapsed.
    //The associated do-loops must be structured blocks. Their execution must not be terminated by a break statement.
    //If multiple loops are associated to the loop construct, only an iteration of the innermost associated loop may be curtailed by a continue statement.
    // If multiple loops are associated to the loop construct, there must be no branches to any of the loop termination statements except for the innermost associated loop.
#pragma omp parallel for collapse(2) num_threads(2)
    for(int x = 0; x <= 2; x++)
        for(int y = 0; y <= 2; y++) {
            std::cout << "ThreadID: " << omp_get_thread_num() << " (" << x << ", " << y << ")\n";
        }
    return 0;
}