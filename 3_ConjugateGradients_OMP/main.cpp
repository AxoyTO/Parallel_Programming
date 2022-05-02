#include <cmath>
#include <iostream>
#include <vector>
#include "functions.hpp"
#include "omp.h"
#include "sparse_matrix.hpp"

#define watch(x) std::cout << (#x) << " is " << (x) << "\n"
#define OUTPUT 1

using namespace CGSolver;

int main(int argc, char** argv) {
  int M;  // M non-null elements in every row
  // i = K * Nx * Ny + J * Nx + I

  // TODO: Change at last
  if (argc < 1) {
    std::cout << "[INFO] Command line format:\n\t."
                 "/<binary> <Nx|Ny|Nz> <number_of_openMP_threads>\n\t"
                 "For example: ./CG.exe 4 \n\t"
                 "[!] M <= N\n";
    return 1;
  } else {
    int N_side = std::stoi(argv[1]);
    // N = Nx * Ny * Nz
    // Sparse Matrix = N x N
    int N = std::pow(N_side, 3);

    Vector row, col, val;
    Matrix A;
    Vector b(N, 0);
    Row_Col_Val_UOMap row_col_val;

    generate_cube_and_sparse_matrix(N_side, A);
    generate_row_col_val(A, row_col_val);
    generate_vector_b(b);

#if OUTPUT == 1
    print_matrix(A);
    print_row_col_val(row_col_val);
    print_vector(b);
#endif

    Matrix r, z;
    Matrix M;  // Preconditioner - diagonal matrix with diagonal of A
    set_diagonal_matrix_M(A, M);
    inverse_diagonal_matrix(M);
    print_matrix(M);

    Vector x(N, 0);
    // Vector r(N, 0);
    // Vector z(N, 0);
    Vector p(N, 0);
    Vector rho(N, 0);
    Vector alpha(N, 0);
    Vector beta(N, 0);

    // x_0 = 0
    int k = 1;
    r.push_back(b);  // r = b - A * x_0
    bool convergence = false;

    while (!convergence) {
      //        std::cout << "Iter: " << k << "   Norm: " << get_norm(r) <<
      //        std::endl;

      z[k] = precondition(M, r[k - 1]);
      //  precond(z, m, r);

      /*spmv(A, p, q);
      scalar1 = dot(r, z);
      scalar2 = dot(p, q);
      alpha = scalar1 / scalar2;

      axpby(alpha, p, 1.0, x);
      axpby(-alpha, q, 1.0, r);

      scalar3 = dot(r, z);
      betta = scalar3 / scalar1;
      axpby(1.0, z, betta, p);
      */
      // if (get_norm(r) < 1e-10 || ++k >= max_solver_iters)
      convergence = true;
    }

    /*if (argc == 1) {
      std::cout << "Default parameters will be used.\n"
                << "You can set custom values to cube size (argument 1),\n"
                << "maximum of solver iterations (argument 2),\n"
                << "number of test runs (argument 3)\n"
                << "and number of warmups (argument 4).\n"
                << "All values must be non-negative\n\n";
    }
    */
  }

  return 0;
}