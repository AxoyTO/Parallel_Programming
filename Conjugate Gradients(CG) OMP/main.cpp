#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include "functions.hpp"
#include "omp.h"

#define watch(x) std::cout << (#x) << " is " << (x) << "\n"
#define OUTPUT 0

constexpr double eps = 10e-15;

using namespace CGSolver;

int main(int argc, char** argv) {
  int M;  // M non-null elements in every row
  // i = K * Nx * Ny + J * Nx + I
  if (argc < 6) {
    std::cout << "[INFO] Command line format:\n\t."
                 "/<binary> <Nx> <Ny> <Nz> <max_iterations_k> "
                 "<number_of_openMP_threads>\n\t"
                 "For example: ./CG.exe 10 10 10 50 12 \n\t"
                 "[!] M <= N\n";
    return 1;
  } else {
    const int Nx = std::stoi(argv[1]);
    const int Ny = std::stoi(argv[2]);
    const int Nz = std::stoi(argv[3]);
    int maxiter = std::stoi(argv[4]);
    int n_threads = std::stoi(argv[5]);
    // N = Nx * Ny * Nz
    // Sparse Matrix = N x N
    int N = Nx * Ny * Nz;

    omp_set_num_threads(n_threads);

    Vector row, col, val;
    Matrix A;
    Vector b(N, 0);
    Row_Col_Val_UOMap row_col_val;

    double time_t1;
    time_t1 = omp_get_wtime();
    generate_cube_and_sparse_matrix(Nx, Ny, Nz, A);
    time_t1 = omp_get_wtime() - time_t1;
    generate_row_col_val(A, row_col_val);
    generate_vector_b(A, b);

#if OUTPUT == 1
    std::streamsize original_precision = std::cout.precision();
    print_matrix(A);
    print_row_col_val(row_col_val);
    print_vector(b);
    std::cout << std::setprecision(original_precision);
    std::cout.unsetf(std::ios_base::fixed);
#endif

    /*
    Matrix M;  // Preconditioner - diagonal matrix with diagonal of A
    set_diagonal_matrix_M(A, M);
    inverse_diagonal_matrix(M);
    */

    Vector M(N, 0);
    set_preconditioner_diagonal(A, M);
    inverse_preconditioner_diagonal(M);

    Vector x(N, 0);
    Vector r(N, 0);
    Vector z(N, 0);
    Vector p(N, 0);
    Vector q(N, 0);

    double alpha;
    double beta;
    double rho, rho_last;  // rho[k-1] is needed

    int k = 1;
    r = b;  // r = b - A * x_0
    bool convergence = false;

    double time_t2, ddot_time, axpby_time, spmv_time, temp_time;
    double total_time = 0, total_ddot_time = 0, total_axpby_time = 0,
           total_spmv_time = 0;

    while (!convergence) {
      time_t2 = omp_get_wtime();

      z = precondition(M, r);
      rho_last = rho;
      ddot_time = omp_get_wtime();
      rho = ddot(r, z);
      ddot_time = omp_get_wtime() - ddot_time;

      if (k == 1)
        p = z;
      else {
        beta = rho / rho_last;
        axpby_time = omp_get_wtime();
        p = axpby(1, z, beta, p);
        axpby_time = omp_get_wtime() - axpby_time;
      }

      spmv_time = omp_get_wtime();
      q = spmv(A, p);
      spmv_time = omp_get_wtime() - spmv_time;

      temp_time = omp_get_wtime();
      alpha = rho / ddot(p, q);
      ddot_time += omp_get_wtime() - temp_time;

      temp_time = omp_get_wtime();
      x = axpby(1, x, alpha, p);
      r = axpby(1, r, -alpha, q);
      axpby_time += omp_get_wtime() - temp_time;

      if (get_norm(r) < eps || k >= maxiter)
        convergence = true;
      else
        k++;
      time_t2 = omp_get_wtime() - time_t2;

      total_time += time_t2;
      total_axpby_time += axpby_time;
      total_ddot_time += ddot_time;
      total_spmv_time += spmv_time;
    }

    std::cout << "Program running on " << omp_get_max_threads()
              << " OMP thread(s).\n";
    std::cout << "Nx:" << Nx << "   Ny:" << Ny << "   Nz:" << Nz << "\n";
    std::cout << "Sparse Matrix Size: " << A.size() << " x " << A.size()
              << "\n";
    std::cout << "Iterations: " << k << " / Max Iterations: " << maxiter
              << "\n";
    std::cout << "Cube and Sparse Matrix generation time: " << time_t1
              << " s.\n";
    std::cout << "Total time of all iterations: " << total_time << " s.\n";
    std::cout << "Average time of an iteration: " << total_time / k << " s.\n";
    std::cout << "Average SpMV time: " << total_spmv_time / k << " s.\n";
    std::cout << "Average axpby time: " << total_axpby_time / k << " s.\n";
    std::cout << "Average ddot time: " << total_ddot_time / k << " s.\n";
    std::cout << "Norm of residual: " << get_norm(r) << "\n";
  }

  std::cout << "\n";
  return 0;
}