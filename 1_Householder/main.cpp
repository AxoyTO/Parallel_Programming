#include <omp.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using std::vector;
using Vector = vector<double>;
using Matrix = vector<Vector>;

/*
int generateRandomNumber() {
  std::random_device device;
  std::mt19937 rng(device());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 9);

  return dist(rng);
}
*/

Matrix generateRandomMatrix(Matrix& matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    matrix.emplace_back();
    for (double j = 0; j < cols; j++) {
      matrix.at(i).push_back(rand() % 10);
    }
  }

  return matrix;
}

Vector generateRandomVector(Vector& vector, int rows) {
  for (int i = 0; i < rows; i++) {
    vector.push_back(rand() % 10);
  }

  return vector;
}

void displayMatrix(const Matrix& matrix) {
  std::cout << "==============\n";
  for (const auto& r : matrix) {
    for (const auto& c : r) {
      std::cout << c << "  ";
    }
    std::cout << "\n";
  }
  std::cout << "==============\n";
}

void displayVector(std::string title, const Vector& vec) {
  std::cout << title << " ";
  for (const auto& v : vec) {
    std::cout << v << " ";
  }
  std::cout << "\n";
}

Vector getColumn(const Matrix& matrix, double k) {
  Vector column;
  for (double r = k; r < matrix.size(); r++) {
    column.emplace_back(matrix[r][k]);
  }

  return column;
}

double euclideanNorm(const Vector& vec) {
  double norm = 0.0;
  // !!!!!!!!!!!!!!!!!!!
  //#pragma omp parallel for reduction(+ : norm)
  for (const auto& v : vec) {
    norm += v * v;
  }
  return std::sqrt(norm);
}

Matrix identityMatrix(int n) {
  Matrix I;

  for (int i = 0; i < n; i++) {
    I.emplace_back(n, 0);
  }

#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    I[i][i] = 1;
  }

  return I;
}

Matrix v_vT_multiplication(Vector v) {
  Matrix vvT;

  for (int i = 0; i < v.size(); i++) {
    vvT.emplace_back(Vector(v.size(), 0));
  }

#pragma omp parallel for collapse(2)
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v.size(); j++) {
      vvT[i][j] = v[i] * v[j];
    }
  }

  return vvT;
}

void matrixMultiplication(const Matrix& H, Matrix& A, int t) {
  Matrix R;

  for (int i = 0; i < H.size(); i++) {
    R.emplace_back(Vector(A[0].size() - t, 0));
  }

#pragma omp parallel for
  for (int i = 0; i < H.size(); i++) {
    for (int j = 0; j < A[0].size() - t; j++) {
      double s = 0;
      for (int k = 0; k < H[0].size(); k++) {
        s += H[i][k] * A[k + t][j + t];
      }
      R[i][j] = s;
      if (std::abs(R[i][j]) < 10e-7) {
        R[i][j] = 0;
      }
    }
  }

#pragma omp parallel for collapse(2)
  for (int i = 0; i < R.size(); i++) {
    for (int j = 0; j < R[0].size(); j++) {
      A[i + t][j + t] = R[i][j];
    }
  }
}

Vector reverseGaussian(Matrix& A, Vector& b) {
  int n = b.size();
  b[n - 1] /= A[n - 1][n - 1];
  for (int i = n - 2; i >= 0; i--) {
    for (int j = n - 1; j > i; j--) {
      b[i] -= b[j] * A[i][j];
    }
    b[i] = b[i] / A[i][i];
  }
  return b;
}

void householderReflection(Matrix& A, Vector& b) {
#pragma omp parallel for
  for (int i = 0; i < A.size(); i++) {
    A[i].push_back(b[i]);
  }

  Matrix I, vvT;

  for (int k = 0; k < A.size(); k++) {
    Vector a = getColumn(A, k);
    double a_norm = euclideanNorm(a);
    if (!a_norm)
      continue;

    Vector unit_vector(a.size(), 0);
    Vector v(a.size(), 0);

    double a1 = A[k][k];
    double sign = (a1 > 0) ? 1 : -1;

    unit_vector[0] = sign * a_norm;

    std::transform(a.begin(), a.end(), unit_vector.begin(), v.begin(),
                   std::plus<double>());

    double v_norm = euclideanNorm(v);

    std::transform(v.begin(), v.end(), v.begin(),
                   [&v_norm](double& c) { return c / v_norm; });

    vvT = v_vT_multiplication(v);

    for (int i = 0; i < vvT.size(); i++) {
      std::transform(vvT[i].begin(), vvT[i].end(), vvT[i].begin(),
                     [](double& c) { return c * -2; });
    }

    I = identityMatrix(a.size());
    for (int i = 0; i < vvT.size(); i++) {
      std::transform(vvT[i].begin(), vvT[i].end(), I[i].begin(), vvT[i].begin(),
                     std::plus<double>());
    }

    matrixMultiplication(vvT, A, k);
  }

  for (int i = 0; i < A.size(); i++) {
    b[i] = A[i][A[i].size() - 1];
    A[i].pop_back();
  }

  int reverse_rank_count = 0;
#pragma omp parallel for
  for (int i = 0; i < A.size(); i++) {
    bool all_zero_row =
        std::all_of(A[i].begin(), A[i].end(), [](double i) { return i == 0; });
    if (all_zero_row)
      reverse_rank_count++;
  }

  for (int i = 0; i < reverse_rank_count; i++) {
    A.pop_back();
    b.pop_back();
  }

  assert(A.size() == A[0].size() && "Infinite solutions exist!");
}

void residualVector(const Matrix& A, const Vector& x, const Vector& b) {
  Vector residual_vector(b.size(), 0);

  for (int i = 0; i < A.size(); i++) {
    double s = 0;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //#pragma omp parallel for reduction(+ : s)
    for (int j = 0; j < A[i].size(); j++) {
      s += A[i][j] * x[j];
    }
    residual_vector[i] = s;
  }

#pragma omp parallel for
  for (int i = 0; i < residual_vector.size(); i++) {
    residual_vector[i] -= b[i];
  }

  std::cout << "Norm of residual: " << euclideanNorm(residual_vector) << "\n";
}

int main(int argc, char** argv) {
  if (argc < 4) {
    std::cout << "Command line format:\n\t."
                 "/<binary> <rows> <columns> <number_of_openMP_threads>\n\t"
                 "For example: ./HouseHolder.exe 1024 1024 4\n";
  } else {
    Matrix init_A, A;
    Vector init_b, b, x;
    double T1, T2;

    int rows = std::stoi(argv[1]);
    int cols = std::stoi(argv[2]);
    int n_threads = std::stoi(argv[3]);
    omp_set_num_threads(n_threads);

    // std::cout << "====== MATRIX: " << rows << " x " << cols
    //           << " --- THREADS: " << n_threads << " ======\n\n";

    init_A = generateRandomMatrix(A, rows, cols);
    init_b = generateRandomVector(b, rows);
    double start = omp_get_wtime();
    householderReflection(A, b);
    T1 = omp_get_wtime() - start;
    std::cout << "Householder Reflection elapsed time(T1): " << T1 << " s.\n";
    start = omp_get_wtime();
    x = reverseGaussian(A, b);
    T2 = omp_get_wtime() - start;
    std::cout << "Reverse Gaussian elapsed time(T2): " << T2 << " s.\n";
    std::cout << "Total time elapsed(T1+T2): " << T1 + T2 << " s.\n";
    residualVector(init_A, x, init_b);
    std::cout << "\n";
  }

  return 0;
}