#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

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
  int sum;
#pragma omp parallel private(i, j, k) shared(a, b, c)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      // #pragma omp single
      // int & sum = c[i][j];
#pragma omp for reduction(+ : c[i][j])
      for (k = 0; k < N; k++) {
        c[i][j] += a[i][k] + b[k][j];
      }
      // #pragma omp single
      //    c[i][j] = sum;
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
}*/
  return 0;
}
