#include "functions.hpp"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>

constexpr double pi = 3.14159265358979323846;

namespace CGSolver {

void generate_cube_and_sparse_matrix(const int Nx,
                                     const int Ny,
                                     const int Nz,
                                     Matrix& sparse_matrix) {
  auto non_diag = [](int i, int j) -> double { return cos(i * j + pi); };
  // N_side = Nx = Ny = Nz
  // i-Nx*Ny if K > 0
  // i-Nx if J > 0
  // i-1 if I > 0
  // i
  // i+1 if I < Nx-1
  // i+Nx if I < Ny-1
  // i+Nx*Ny if K < Nz-1

  for (int K = 0; K < Nz; K++) {
    for (int J = 0; J < Ny; J++) {
      for (int I = 0; I < Nx; I++) {
        Vector row(Nx * Ny * Nz, 0);
        int i = K * Nx * Ny + J * Nx + I;
        int j = {};
        double non_diag_value = 0, sum = 0;
        // i-Nx*Ny if K > 0
        if (K > 0) {
          j = i - Nx * Ny;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i-Nx if J > 0
        if (J > 0) {
          j = i - Nx;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i-1 if I > 0
        if (I > 0) {
          j = i - 1;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i+1 if I < Nx-1
        if (I < Nx - 1) {
          j = i + 1;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i+Nx if I < Ny-1
        if (J < Ny - 1) {
          j = i + Nx;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i+Nx*Ny if K < Nz-1
        if (K < Nz - 1) {
          j = i + Nx * Ny;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i always
        if (true) {
          j = i;
          row[j] = (Nx != 1 || Ny != 1 || Nz != 1) ? (sum * 1.5) : 1;
        }
        sparse_matrix.push_back(row);
      }
    }
  }
}

void generate_row_col_val(const Matrix& sparse_matrix,
                          Row_Col_Val_UOMap& row_col_val) {
  for (int i = 0; i < sparse_matrix.size(); i++) {
    Vector col, val;
    for (int j = 0; j < sparse_matrix[i].size(); j++) {
      if (sparse_matrix[i][j] != 0) {
        col.push_back(j);
        val.push_back(sparse_matrix[i][j]);
      }
    }
    row_col_val[i] = std::make_pair(col, val);
  }
}

void generate_vector_b(const Matrix& matrix, Vector& b) {
  const int max_connections = (b.size() > 8) ? 7 : ((b.size() == 1) ? 1 : 4);

  for (int i = 0; i < matrix.size(); i++) {
    double row_sum = 0;
    //#pragma omp parallel for reduction(+ : row_sum)
    for (const auto& i : matrix[i]) {
      if (i != 0)
        row_sum += i;
    }
    b[i] = row_sum;
    // b[i] = rand() % 10;
  }
}

void set_preconditioner_diagonal(const Matrix& A, Vector& M) {
  for (int i = 0; i < A.size(); i++) {
    M[i] = A[i][i];
  }

  assert(A.size() == M.size() &&
         "Sizes of the sparse matrix and preconditioner M are incompatible!");
}

void inverse_preconditioner_diagonal(Vector& M) {
  //#pragma omp parallel for
  for (int i = 0; i < M.size(); i++) {
    M[i] = 1 / M[i];
  }
}

/*
void set_diagonal_matrix_M(const Matrix& A, Matrix& M) {
  for (int i = 0; i < A.size(); i++) {
    M.push_back(Vector(A[i].size(), 0));
    for (int j = 0; j < A[i].size(); j++) {
      if (i == j) {
        M[i][j] = A[i][j];
      }
    }
  }

  assert(A.size() == M.size() &&
         "Sizes of the sparse matrix and preconditioner M are incompatible!");
}

void inverse_diagonal_matrix(Matrix& M) {
  //#pragma omp parallel for
  for (int i = 0; i < M.size(); i++) {
    assert(M.size() == M[i].size() && "Matrix is not in square form!");
    M[i][i] = 1 / M[i][i];
  }
}

Vector precondition(const Matrix& M, const Vector& r) {
  Vector z;
  for (int i = 0; i < M.size(); i++) {
    z.push_back(M[i][i] * r[i]);
  }
  return z;
}

*/
Vector precondition(const Vector& M, const Vector& r) {
  Vector z;
  for (int i = 0; i < M.size(); i++) {
    z.push_back(M[i] * r[i]);
  }
  return z;
}

double ddot(const Vector& v1, const Vector& v2) {
  double sum = 0;
  assert(v1.size() == v2.size() && "Vectors are not of same length!");

#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < v1.size(); i++) {
    sum += v1[i] * v2[i];
  }

  return sum;
}

Vector axpby(const double alpha,
             const Vector& v1,
             const double beta,
             const Vector& v2) {
  assert(v1.size() == v2.size() && "axpby vectors are not of same length!");

  Vector v(v1.size(), 0);

#pragma omp parallel for
  for (int i = 0; i < v1.size(); i++)
    v[i] = alpha * v1[i] + beta * v2[i];

  return v;
}

Vector spmv(const Matrix& matrix, const Vector& vec) {
  assert(matrix.size() == vec.size() &&
         "spmv matrix and vector are incompatible!");

  Vector res(matrix.size(), 0);

#pragma omp parallel for
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      res[i] += matrix[i][j] * vec[j];
    }
  }

  return res;
}

double get_norm(const Vector& vec) {
  double norm = 0;

  /*
  POLUS error
  #pragma omp parallel for reduction(+ : norm)
    for (const auto& i : vec)
      norm += i * i;
  */

#pragma omp parallel for reduction(+ : norm)
  for (int i = 0; i < vec.size(); i++)
    norm += vec[i] * vec[i];

  // std::cout << sqrt(norm) << "\n";
  return std::sqrt(norm);
}

// Ax-b
Vector get_residual(const Matrix& A, const Vector& x, const Vector& b) {
  Vector residual(x.size(), 0);
  residual = spmv(A, x);

  for (int i = 0; i < residual.size(); i++)
    residual[i] -= b[i];

  return residual;
}

void print_matrix(const Matrix& sparse_matrix) {
  std::cout << std::setprecision(2);
  std::cout << "==================== MATRIX ====================\n";
  for (const auto& row : sparse_matrix) {
    for (const auto& i : row) {
      std::cout << std::fixed << std::setw(5) << i << " ";
    }
    std::cout << "\n";
  }
  std::cout << "================================================\n\n";
}

void print_vector(const Vector& vec) {
  std::cout << "==================== VECTOR ====================\n";
  for (const auto& i : vec) {
    std::cout << i << " ";
  }
  std::cout << "\n================================================\n\n";
}

void print_row_col_val(const Row_Col_Val_UOMap& row_col_val) {
  std::cout << std::setprecision(2);

  // If N_side is 2(N is 8), then max 4 connections. Else 7
  const int max_connections =
      (row_col_val.size() > 8) ? 7 : ((row_col_val.size() == 1) ? 1 : 4);

  std::cout << "Row  ";

  if (max_connections != 7) {
    for (int i = 0; i < row_col_val.at(0).first.size(); i++) {
      std::cout << "Col:Val" << std::setw(16);
    }
    std::cout << "\n---";
    for (int i = 0; i < row_col_val.at(0).first.size(); i++) {
      std::cout << "  --------" << std::setw(16);
    }
    std::cout << "\n";
  } else {
    for (int i = 0; i < 7; i++) {
      std::cout << "Col:Val" << std::setw(16);
    }
    std::cout << "\n---";
    for (int i = 0; i < 7; i++) {
      std::cout << "  --------" << std::setw(16);
    }
    std::cout << "\n";
  }
  for (int i = 0; i < row_col_val.size(); i++) {
    std::cout << std::setw(2) << i << "   ";
    for (int j = 0; j < row_col_val.at(i).first.size(); j++) {
      /*
      std::cout << std::defaultfloat << row_col_val.at(i).first[j] << ":"
                << std::fixed << std::setw(5) << row_col_val.at(i).second[j]
                << std::setw(10);
                */
      std::cout << row_col_val.at(i).first[j] << ":" << std::fixed
                << std::setw(5) << row_col_val.at(i).second[j] << std::setw(10);
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

}  // namespace CGSolver