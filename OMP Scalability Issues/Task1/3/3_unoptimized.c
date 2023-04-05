#include <omp.h>
#include <stdio.h>

int main() {
  const int n = 1000000000;
  int i, count = 0;
  int x, y;
  double num;
  double start_time, end_time;

  start_time = omp_get_wtime();

#pragma omp parallel for private(x, y) reduction(+ : count)
  for (i = 0; i < n; ++i) {
    x = i;
    y = i + 1;
    if (x * x + y * y <= 1) {
#pragma omp critical
      count++;
    }
  }

  num = 4.0 * count / n;

  end_time = omp_get_wtime();

  //printf("num: %f\n", num);

  printf("Time: %lf seconds for %d threads.\n", end_time - start_time,
         omp_get_max_threads());

  return 0;
}
