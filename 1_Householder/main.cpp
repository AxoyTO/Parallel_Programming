#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using std::vector;
using clk = std::chrono::high_resolution_clock;
using sec = std::chrono::duration<double>;
using Vector = vector<double>;
using Matrix = vector<Vector>;

int generateRandomNumber() {
  std::random_device device;
  std::mt19937 rng(device());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 9);

  return dist(rng);
}

Matrix generateRandomMatrix(Matrix& matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    matrix.emplace_back();
    for (double j = 0; j < cols; j++) {
      matrix.at(i).push_back(generateRandomNumber());
    }
  }

  return matrix;
}

Vector generateRandomVector(Vector& vector, int rows) {
  for (int i = 0; i < rows; i++) {
    vector.push_back(generateRandomNumber());
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
  double norm = 0;
  for (const auto& v : vec) {
    norm += v * v;
  }
  return std::sqrt(norm);
}

Matrix identityMatrix(int n) {
  Matrix I;
  for (int i = 0; i < n; i++) {
    Vector row(n, 0);
    row[i] = 1;
    I.emplace_back(row);
  }

  return I;
}

Matrix v_vT_multiplication(Vector v) {
  Matrix vvT;
  for (int i = 0; i < v.size(); i++) {
    Vector row;
    for (int j = 0; j < v.size(); j++) {
      row.push_back(v[i] * v[j]);
    }
    vvT.emplace_back(row);
  }

  return vvT;
}

void matrixMultiplication(const Matrix& H, Matrix& A, int t) {
  Matrix R;
  int i, j, k;

  for (i = 0; i < H.size(); ++i) {
    R.emplace_back(Vector(A[0].size() - t, 0));
    for (j = 0; j < A[0].size() - t; ++j) {
      for (k = 0; k < H[0].size(); ++k) {
        R[i][j] += H[i][k] * A[k + t][j + t];
      }
      if (std::abs(R[i][j]) < 10e-8) {
        R[i][j] = 0;
      }
    }
  }

  for (int i = 0; i < R.size(); i++) {
    for (int j = 0; j < R[i].size(); j++) {
      A[i + t][j + t] = R[i][j];
    }
  }
}

void reverseGaussian(Matrix& A, Vector& b) {
  b[0] = 4;
  b[1] = -8;
  b[2] = -3;
  A[0][0] = 6;
  A[0][1] = 8;
  A[0][2] = 4;
  A[1][1] = -10;
  A[1][2] = -4;
  // A[2][1] = 0;
  A[2][2] = -2;

  int n = b.size();
  b[n - 1] /= A[n - 1][n - 1];
  for (int i = n - 2; i >= 0; i--) {
    double s = 0.0;
    for (int j = n - 1; j > i; j--) {
      b[i] -= b[j] * A[i][j];
    }
    b[i] = b[i] / A[i][i];
  }
  displayVector("b:", b);
}

void householderReflection(Matrix& A, Vector& b) {
  for (int i = 0; i < A.size(); i++) {
    A[i].push_back(b[i]);
  }

  displayMatrix(A);

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
                   [&v_norm](auto& c) { return c / v_norm; });

    vvT = v_vT_multiplication(v);

    for (int i = 0; i < vvT.size(); i++) {
      std::transform(vvT[i].begin(), vvT[i].end(), vvT[i].begin(),
                     [](auto& c) { return c * -2; });
    }

    I = identityMatrix(a.size());
    for (int i = 0; i < vvT.size(); i++) {
      std::transform(vvT[i].begin(), vvT[i].end(), I[i].begin(), vvT[i].begin(),
                     std::plus<double>());
    }

    matrixMultiplication(vvT, A, k);
  }

  // displayMatrix(A);
  for (int i = 0; i < A.size(); i++) {
    b[i] = A[i][A[i].size() - 1];
    A[i].pop_back();
  }

  int reverse_rank_count = 0;
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
  displayVector("newB:", b);
  displayMatrix(A);

  // Vector x = reverseGaussian(A, b);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "Command line format:\n\t."
                 "/<binary> <rows> <columns> <number_of_openMP_threads>\n\t"
                 "For example: ./HouseHolder.exe 1024 1024 4\n";
  } else {
    Matrix init_A, A;
    Vector init_b, b;
    int rows = std::stoi(argv[1]);
    int cols = std::stoi(argv[2]);
    init_A = generateRandomMatrix(A, rows, cols);
    init_b = generateRandomVector(b, rows);
    auto start = clk::now();
    householderReflection(A, b);
    sec duration = clk::now() - start;
    std::cout << "Householder Reflection elapsed time: " << duration.count()
              << " s.\n";
    start = clk::now();
    reverseGaussian(A, b);
    duration = clk::now() - start;
    std::cout << "Reverse Gaussian elapsed time: " << duration.count()
              << " s.\n";
  }

  return 0;
}