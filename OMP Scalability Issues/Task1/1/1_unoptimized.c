#include <omp.h>
#include <stdio.h>

#define N 5000

int main() {
  int a[N][N], b[N][N], c[N][N];
  int i, j, k;
  double start_time, end_time;

  // Инициализация матриц
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = i - j;
      c[i][j] = 0;
    }
  }

  start_time = omp_get_wtime();

// Параллельный умножение матриц
#pragma omp parallel for private(i, j, k) shared(a, b, c)
  for (k = 0; k < N; k++) {
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }

  end_time = omp_get_wtime();

  // Вывод времени выполнения и результатов
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
