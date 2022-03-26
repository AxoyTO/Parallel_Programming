#pragma once
#include <mpi.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

struct Column {
  double* col;
  int rank;
};

using Vector = std::vector<double>;
using Matrix = std::vector<Column>;

double random_number_generator();

void generate_random_matrix(Matrix& init_A,
                            Matrix& A,
                            const int N,
                            int rank,
                            int comm_size);

void generate_random_vector(Vector& init_b,
                            Vector& b,
                            const Matrix& A,
                            const int N);

void print_matrix(Matrix A, int N, int rank, int comm_size);

// double euclidean_norm(const Vector& vec);

void householder_reflection(Matrix& A,
                            Vector& b,
                            int N,
                            int rank,
                            int comm_size);

void reverse_gaussian(Matrix& A,
                      Vector& b,
                      std::vector<std::pair<int, double>>& res,
                      int N,
                      int rank,
                      int comm_size);

void gather_results(std::vector<std::pair<int, double>>& res,
                    int N,
                    int rank,
                    int comm_size);

double residual(Matrix& init_A,
                Vector& init_b,
                std::vector<std::pair<int, double>>& res,
                int N,
                int rank,
                int comm_size);

void allocate_matrix(Matrix& A, const int N, int rank, int comm_size);

void free_matrix(Matrix& columns);

void print_results(const int comm_size,
                   const int N,
                   const double t,
                   const double T1,
                   const double T2);