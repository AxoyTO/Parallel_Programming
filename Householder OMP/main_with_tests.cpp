#include <omp.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <typeinfo>
#include <vector>

#define watch(x) std::cout << (#x) << " is " << (x) << "\n"

#define TESTS 1

using std::vector;
using Vector = vector<double>;
using Matrix = vector<Vector>;

Matrix generateRandomMatrix(Matrix& matrix, const int rows, const int cols) {
  for (int i = 0; i < rows; i++) {
    matrix.emplace_back();
    for (double j = 0; j < cols; j++) {
      matrix.at(i).push_back(rand() % 10);
    }
  }

  return matrix;
}

Vector generateRandomVector(Vector& vector, const int rows) {
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

void displayVector(const std::string title, const Vector& vec) {
  std::cout << title << " ";
  for (const auto& v : vec) {
    std::cout << v << " ";
  }
  std::cout << "\n";
}

Vector getColumn(const Matrix& matrix, const double k) {
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

Matrix identityMatrix(const int n) {
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

Matrix v_vT_multiplication(const Vector& v) {
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

void matrixMultiplication(const Matrix& H, Matrix& A, const int t) {
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

Vector reverseGaussian(const Matrix& A, Vector& b) {
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

#if TESTS == 0
  assert(A.size() == A[0].size() && "Infinite solutions exist!");
#else
  if (A.size() != A[0].size())
    throw std::exception();
#endif
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

class Tests {
 public:
  Tests(int rows, int cols) : rows_(rows), cols_(cols) {}

  Matrix generateRandomTestMatrix(const int test_number) {
    assert(test_number > 0 && "There is no such test!");
    assert(test_number < 3 && "There is no such test!");

    switch (test_number) {
      case 1:
        for (int i = 0; i < rows_; i++) {
          A_.emplace_back();
          for (double j = 0; j < cols_; j++) {
            A_.at(i).push_back(i + j + 1);
          }
        }
        break;
      case 2:
        for (int i = 0; i < rows_; i++) {
          A_.emplace_back();
          for (double j = 0; j < cols_; j++) {
            A_.at(i).push_back(rand() % 100 * 0.1);
          }
        }
        break;
    }

    return A_;
  }

  Vector generateRandomTestVector() {
    for (int i = 0; i < rows_; i++) {
      double temp = 0;
      for (int j = 0; j < cols_; j++) {
        temp += A_[i][j];
      }
      b_.push_back(temp);
    }

    return b_;
  }

  void reset() {
    for (int i = 0; i < A_.size(); i++) {
      A_[i].clear();
    }
    A_.clear();
    b_.clear();
    x_.clear();
  }

  bool test1() {
    reset();
    std::cout << "Running TEST 1 --> ";
    generateRandomTestMatrix(1);
    generateRandomTestVector();
    try {
      householderReflection(A_, b_);
      //Матрица вырожденная -> Программа должна падать в функции
    } catch (std::exception& ex) {
      return false;
    }
    return true;
  }

  bool test2() {
    reset();
    std::cout << "Running TEST 2 --> \n";
    generateRandomTestMatrix(2);
    generateRandomTestVector();

    householderReflection(A_, b_);
    x_ = reverseGaussian(A_, b_);

    const Vector exact_x(rows_, 1);
    Vector test_result(rows_, 0);

    for (int i = 0; i < x_.size(); i++) {
      test_result[i] = exact_x[i] - x_[i];
    }

    std::cout << "||x(точное)-x||: " << euclideanNorm(test_result) << "\n";
    for (const auto& v : x_) {
      if (int(std::round(v)) != 1) {
        return false;
      }
    }
    return true;
  }

 private:
  int rows_, cols_;
  Matrix A_;
  Vector x_, b_;
};

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

#if TESTS == 1
    std::cout << "Running tests.\n";
    Tests tests(rows, cols);
    if (tests.test1() == false)
      std::cout << "TEST PASSED!\n";
    else
      std::cout << "TEST FAILED...\n";
    if (tests.test2() == true)
      std::cout << "\n";
    else
      std::cout << "TEST FAILED...\n";
#else
    init_A = generateRandomMatrix(A, rows, cols);
    displayMatrix(A);
    init_b = generateRandomVector(b, rows);

    displayVector("Generated Vector: ", b);
    double start = omp_get_wtime();
    householderReflection(A, b);
    T1 = omp_get_wtime() - start;
    std::cout << "Householder Reflection elapsed time(T1): " << T1 << " s.\n";
    start = omp_get_wtime();
    x = reverseGaussian(A, b);
    T2 = omp_get_wtime() - start;

    displayVector("x: ", x);
    std::cout << "Norm of x: " << euclideanNorm(x) << "\n";

    std::cout << "Reverse Gaussian elapsed time(T2): " << T2 << " s.\n";
    std::cout << "Total time elapsed(T1+T2): " << T1 + T2 << " s.\n";
    residualVector(init_A, x, init_b);
    std::cout << "\n";
#endif
  }

  return 0;
}