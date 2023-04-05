#include <omp.h>
#include <stdio.h>

#define N 1000000

int main() {
  int i, j, sum = 0;
  double start_time, end_time;

  start_time = omp_get_wtime();

#pragma omp parallel for shared(sum) private(i, j) schedule(dynamic, 10000)
  for (i = 0; i < N; i++) {
    int local_sum = 0;
    for (j = 0; j < 10000; j++) {
      local_sum++;
    }

#pragma omp atomic
    sum += local_sum;
  }

  end_time = omp_get_wtime();

  // printf("Sum: %d\n", sum);
  printf("Time: %lf seconds for %d threads.\n", end_time - start_time,
         omp_get_max_threads());

  return 0;
}
