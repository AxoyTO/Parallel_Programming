#include <mpi.h>
#include <iostream>
#include "functions.hpp"
#include "tests.hpp"

#define watch(x) std::cout << (#x) << " is " << (x) << "\n"
#define TEST 0
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

    int N = std::stoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    double T, B, T1, T2, temp_reduction_time, temp_B;

    // T = Total time of serial execution
    // B = Total time of non - parallizable part
    // T-B = Total time of parallizable part(when executed serially)
    // T = B + (T-B)

    if (comm_size == 1) {
      B = 0;
      T = MPI_Wtime();
      temp_B = MPI_Wtime();
    }

    if (check_input_data(N, comm_size)) {
      if (rank == 0)
        std::cout << "[ERROR] Processes "
                     "can't be more than the "
                     "size of matrix!\n";
      MPI_Finalize();
      return 0;
    }

    Matrix init_A, A;
    Vector init_b(N), b(N), x(N);
    std::vector<std::pair<int, double>> res;

    generate_random_matrix(init_A, A, N, rank, comm_size);
    generate_random_vector(init_b, b, init_A, N);

    if (comm_size == 1) {
      B += MPI_Wtime() - temp_B;
    }

#if DEBUG
    print_matrix(init_A, N, rank, comm_size);
    print_vector(init_b, rank);
    // print_matrix(A, N, rank,
    // comm_size);
#endif

    T1 = MPI_Wtime();
    householder_reflection(A, b, N, rank, comm_size);

#if TEST
    if (check_matrix_singularity(A, N, rank, comm_size)) {
      if (rank == 0)
        std::cout << "[INFO] Infinite "
                     "solutions exist!\n";

      free_matrix(init_A);
      free_matrix(A);
      MPI_Finalize();
      return 0;
    }
#endif

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
    // print_matrix(init_A, N, rank,
    // comm_size);
    print_matrix(A, N, rank, comm_size);
#endif

    gather_results(res, N, rank, comm_size);

    double residual_norm =
        norm_of_residual_vector(init_A, init_b, res, N, rank, comm_size);

#if RESULT_OUTPUT
    if (rank == 0) {
      print_results(comm_size, N, residual_norm, T1, T2);
    }
#endif
    if (comm_size == 1)
      temp_B = MPI_Wtime();

    free_matrix(init_A);
    free_matrix(A);

    if (comm_size == 1)
      B += MPI_Wtime() - temp_B;

    if (comm_size == 1) {
      T = MPI_Wtime() - T;
      if (rank == 0)
        std::cout << "Total time of serial "
                     "execution: "
                  << T << " s.\n";
      std::cout << "Total time of non-parallelizable part: " << B << " s.\n";
      std::cout << "Serial / Parallel: " << B / T << "\n";
    }

    MPI_Finalize();
    return 0;
  }
}