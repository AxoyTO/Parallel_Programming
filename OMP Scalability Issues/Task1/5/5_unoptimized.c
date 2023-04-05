#include <omp.h>
#include <stdio.h>

#define N 100000

int main() {
  int i, j, k;
  double A[N][N];
  double start_time, end_time;

  start_time = omp_get_wtime();

#pragma omp parallel for shared(A) private(i, j, k)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (j <= i) {
        A[i][j] = i * j;
      } else {
        A[i][j] = 0;
      }
    }
  }

  end_time = omp_get_wtime();

  printf("Time: %lf seconds for %d threads.\n", end_time - start_time,
         omp_get_max_threads());

  return 0;
}
