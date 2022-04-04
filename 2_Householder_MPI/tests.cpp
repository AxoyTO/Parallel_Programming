#include "tests.hpp"

int check_matrix_singularity(const Matrix& columns,
                             const int N,
                             int rank,
                             int comm_size) {
  int error_flag = 0;
  for (int i = 0; i < columns.size(); i++)
    if (std::abs(columns[i].col[columns[i].rank]) < 10e-7) {
      error_flag = 1;
      break;
    }

  int tmp;
  MPI_Allreduce(&error_flag, &tmp, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  return tmp;
}