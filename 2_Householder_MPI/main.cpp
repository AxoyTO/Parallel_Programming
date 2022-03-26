#include <mpi.h>
#include <iostream>
#include "functions.hpp"

#define watch(x) std::cout << (#x) << " is " << (x) << "\n"
#define DEBUG 0
#define RESULT_OUTPUT 1

using std::vector;

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "[INFO] Command line format:\n\t."
                 "/<binary> <N (Size)>\n\t"
                 "For example: ./Householder.exe 1024\n";
  } else {
    int rank, comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    int N = std::stoi(argv[1]);

    if (comm_size > N) {
      if (rank == 0)
        std::cout
            << "[ERROR] Processes can't be more than the size of matrix!\n\n";
      MPI_Finalize();
      return 0;
    }

    Matrix init_A, A;
    Vector init_b(N), b(N), x(N);
    std::vector<std::pair<int, double>> res;
    double T1, T2, temp_reduction_time;

    generate_random_matrix(init_A, A, N, rank, comm_size);
    generate_random_vector(init_b, b, init_A, N);

#if DEBUG
    print_matrix(init_A, N, rank, comm_size);
    print_matrix(A, N, rank, comm_size);
#endif

    T1 = MPI_Wtime();
    householder_reflection(A, b, N, rank, comm_size);
    T1 = MPI_Wtime() - T1;
    MPI_Allreduce(&T1, &temp_reduction_time, 1, MPI_DOUBLE, MPI_MAX,
                  MPI_COMM_WORLD);
    T1 = temp_reduction_time;

    T2 = MPI_Wtime();
    reverse_gaussian(A, b, res, N, rank, comm_size);
    T2 = MPI_Wtime() - T2;
    MPI_Allreduce(&T2, &temp_reduction_time, 1, MPI_DOUBLE, MPI_MAX,
                  MPI_COMM_WORLD);
    T2 = temp_reduction_time;

#if DEBUG
    print_matrix(A, N, rank, comm_size);
    print_matrix(init_A, N, rank, comm_size);
#endif

    gather_results(res, N, rank, comm_size);

    double t = residual(init_A, init_b, res, N, rank, comm_size);

#if RESULT_OUTPUT
    if (rank == 0) {
      print_results(comm_size, N, t, T1, T2);
    }
#endif
    free_matrix(init_A);
    MPI_Finalize();
    return 0;
  }
}