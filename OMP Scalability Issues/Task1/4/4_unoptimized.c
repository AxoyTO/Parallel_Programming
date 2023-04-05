#include <omp.h>
#include <stdio.h>

#define N 100000000

int main() {
  int sum = 0, i;
  double start_time, end_time;

  start_time = omp_get_wtime();

#pragma omp parallel
  {
#pragma omp for
    for (int i = 0; i < N; i++) {
#pragma omp critical
      sum += 1;
    }
  }

  end_time = omp_get_wtime();

  // printf("Sum = %d\n", sum);
  printf("Time: %lf seconds for %d threads.\n", end_time - start_time,
         omp_get_max_threads());

  return 0;
}
