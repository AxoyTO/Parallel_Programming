#include <omp.h>
#include <stdio.h>

#define N 1000

int main() {
  int a[N][N], b[N][N], c[N][N];
  int i, j, k;
  double start_time, end_time;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = i - j;
      c[i][j] = 0;
    }
  }

  start_time = omp_get_wtime();

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      double sum = 0;
#pragma omp parallel for private(k) shared(a, b, c) reduction(+ : sum)
      for (k = 0; k < N; k++) {
        sum += a[i][k] * b[k][j];
      }
      c[i][j] = sum;
    }
  }

  end_time = omp_get_wtime();

  printf("Time: %lf seconds for %d threads.\n", end_time - start_time,
         omp_get_max_threads());
  /*
printf("Result:\n");
for (i = 0; i < N; i++) {
for (j = 0; j < N; j++) {
printf("%d ", c[i][j]);
}
printf("\n");
}
*/

  return 0;
}
