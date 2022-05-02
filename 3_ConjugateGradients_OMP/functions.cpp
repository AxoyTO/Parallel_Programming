#pragma once

#include "functions.hpp"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>

constexpr double pi = 3.14159265358979323846;

namespace CGSolver {

void generate_cube_and_sparse_matrix(const int N_side, Matrix& sparse_matrix) {
  auto non_diag = [](int i, int j) -> double { return cos(i * j + pi); };
  // N_side = Nx = Ny = Nz
  // i-Nx*Ny if K > 0
  // i-Nx if J > 0
  // i-1 if I > 0
  // i
  // i+1 if I < Nx-1
  // i+Nx if I < Ny-1
  // i+Nx*Ny if K < Nz-1

  for (int K = 0; K < N_side; K++) {
    for (int J = 0; J < N_side; J++) {
      for (int I = 0; I < N_side; I++) {
        Vector row(N_side * N_side * N_side, 0);
        int i = K * N_side * N_side + J * N_side + I;
        int j = {};
        double non_diag_value = 0, sum = 0;
        // i-Nx*Ny if K > 0
        if (K > 0) {
          j = i - N_side * N_side;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i-Nx if J > 0
        if (J > 0) {
          j = i - N_side;
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
        if (I < N_side - 1) {
          j = i + 1;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i+Nx if I < Ny-1
        if (J < N_side - 1) {
          j = i + N_side;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i+Nx*Ny if K < Nz-1
        if (K < N_side - 1) {
          j = i + N_side * N_side;
          non_diag_value = non_diag(i, j);
          row[j] = non_diag(i, j);
          sum += non_diag_value;
        }
        // i always
        if (true) {
          j = i;
          row[j] = N_side != 1 ? (sum * 1.5) : 1;
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
        // row.push_back(i);
        col.push_back(j);
        // col.push_back(j);
        // val.push_back(sparse_matrix[i][j]);
        val.push_back(sparse_matrix[i][j]);
      }
    }
    row_col_val[i] = std::make_pair(col, val);
  }
}

void generate_vector_b(Vector& b) {
  const int max_connections = (b.size() > 8) ? 7 : ((b.size() == 1) ? 1 : 4);

  for (int i = 0; i < b.size(); i++) {
    b[i] = rand() % 10;
  }

  // return b;
}

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
  for (int i = 0; i < M.size(); i++) {
    for (int j = 0; j < M[i].size(); j++) {
      M[i][j] = i == j ? 1 / M[i][j] : M[i][j];
    }
  }
}

void precondition(std::vector<double>& z,
                  const std::vector<double>& diag,
                  const std::vector<double>& r) {
  double point = 0.0;

  for (uint32_t j = 0; j < r.size(); j++)
    point += r[j] * diag[j];

  // set_const(z, point);
}

Vector precondition(const Matrix& M, const Vector& r){
  for(int i = 0; i<)
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
      std::cout << std::defaultfloat << row_col_val.at(i).first[j] << ":"
                << std::fixed << std::setw(5) << row_col_val.at(i).second[j]
                << std::setw(10);
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

}  // namespace CGSolver