#include "tests.hpp"

int check_input_data(const int N, const int comm_size) {
  int error_flag = 0;
  if (comm_size > N) {
    error_flag = 1;
  }

  int tmp;
  MPI_Allreduce(&error_flag, &tmp, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  return tmp;
}

int check_matrix_singularity(const Matrix& A,
                             const int N,
                             int rank,
                             int comm_size) {
  int error_flag = 0;
  for (int i = 0; i < A.size(); i++) {
    // std::cout << "rank:" << A[i].rank << "   " <<
    // std::abs(A[i].col[A[i].rank])
    //       << "\n";
    if (std::abs(A[i].col[A[i].rank]) < 10e-7) {
      error_flag = 1;
      break;
    }
  }

  int tmp;
  MPI_Allreduce(&error_flag, &tmp, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  return tmp;
}
