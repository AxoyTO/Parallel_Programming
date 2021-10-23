//Reduction clause

#include <cmath>
#include <iomanip>
#include <iostream>
#include <omp.h>
#define N 1000000000

int main() {
    double calc = 0;
#pragma omp parallel for reduction(+:calc) num_threads(128)
    for(long i = 0; i < N; i++) {
        calc += pow(-1, i) * 1.0 / (double) (2 * i + 1);
    }

    std::cout << std::setprecision(10);
    std::cout << 4 * calc << "\n";
    return 0;
}
