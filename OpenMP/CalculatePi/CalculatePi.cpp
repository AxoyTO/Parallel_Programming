#include <iomanip>
#include <iostream>
#include <omp.h>

#define N 1000000000
// Partitioning, the bigger N is, the more precise the result!

int main() {
    double h = 1.0 / N;
    double sum = 0.0;
    double x, pi;

#pragma omp parallel default(none) shared(h, sum) private(x) num_threads(1000)
    {
#pragma omp for reduction(+:sum) schedule(static, N/omp_get_num_threads())
        for(long i = 0; i < N; i++) {
            x = (double)i * h;
            sum += 4.0 / (1.0 + x * x);
        }
    }

    pi = h * sum;
    std::cout << std::setprecision(30);
    std::cout << "Calculated Pi: " << pi << "\n";
}