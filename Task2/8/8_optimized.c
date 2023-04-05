#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Функция для вывода треугольной матрицы
void print_matrix(int matrix_size) {
  printf("Matrix:\n");
  for (int i = 0; i < matrix_size; i++) {
    for (int j = 0; j <= i; j++) {
      printf("%4d ", 1);
    }
    printf("\n");
  }
}

int main(int argc, char** argv) {
  int size, rank, i, j, local_sum = 0, global_sum = 0;
  int matrix_size = 1000000;
  double start_time, end_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Засекаем время начала выполнения
  if (rank == 0) {
    // print_matrix(matrix_size);
    start_time = MPI_Wtime();
  }

  // Вычисление суммы элементов треугольной матрицы
  for (i = rank; i < matrix_size; i += size) {
    for (j = 0; j <= i; j++) {
      local_sum += 1;
    }
    // printf("Rank %d processed row %d, local sum: %d\n", rank, i, local_sum);
  }

  // Сбор локальных сумм и получение глобальной суммы
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // Вывод результатов и засеченного времени
  if (rank == 0) {
    end_time = MPI_Wtime();
    printf("Matrix size: %d\n", matrix_size);
    printf("Total sum: %d\n", global_sum);
    printf("Execution time: %f seconds\n", end_time - start_time);
  }

  MPI_Finalize();
  return 0;
}
