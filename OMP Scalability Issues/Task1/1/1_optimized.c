#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5000

int main() {
  int a[N][N], b[N][N], c[N][N];
  int i, j, k;
  double start_time, end_time;

  // Заполнение массивов случайными значениями
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = i - j;
      c[i][j] = 0;
    }
  }

  // Вычисление произведения матриц a и b
  start_time = omp_get_wtime();

#pragma omp parallel shared(a, b, c) private(i, j, k)
  {
#pragma omp for
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        double sum = 0.0;
        for (k = 0; k < N; k++) {
          sum += a[i][k] * b[k][j];
        }
        c[i][j] = sum;
      }
    }
  }

  end_time = omp_get_wtime();

  printf("Time: %lf seconds for %d threads.\n", end_time - start_time,
         omp_get_max_threads());

  return 0;
}
