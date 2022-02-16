#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

using std::vector;

int generateRandomNumber() {
  std::random_device device;
  std::mt19937 rng(device());
  std::uniform_int_distribution<std::mt19937::result_type> dist(
      1, 9);  // distribution in range [1, 6]

  return dist(rng);
}

void generateRandomMatrix(vector<vector<double>>& matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    matrix.emplace_back();
    for (double j = 0; j < cols; j++) {
      matrix.at(i).push_back(generateRandomNumber());
    }
  }
  /*
  matrix.emplace_back(vector<double>{5, 6, 9, 7});
  matrix.emplace_back(vector<double>{2, 7, 1, 8});
  matrix.emplace_back(vector<double>{8, 6, 7, 1});
  matrix.emplace_back(vector<double>{6, 7, 9, 3});
  */
}

template <typename T>
void displayMatrix(vector<vector<T>>& matrix) {
  std::cout << "==============\n";
  for (const auto& r : matrix) {
    for (const auto& c : r) {
      std::cout << c << "  ";
    }
    std::cout << "\n";
  }
  std::cout << "==============\n";
}

void displayVector(std::string title, const vector<double>& vec) {
  std::cout << title << " ";
  for (const auto& v : vec) {
    std::cout << v << " ";
  }
  std::cout << "\n";
}

vector<double> getColumn(const vector<vector<double>>& matrix, double k) {
  vector<double> column;
  for (double r = k; r < matrix.size(); r++) {
    column.emplace_back(matrix[r][k]);
  }

  return column;
}

double euclideanNorm(const vector<double>& vec) {
  double norm = 0;
  for (const auto& v : vec) {
    norm += v * v;
  }
  return std::sqrt(norm);
}

vector<vector<double>> identityMatrix(int n) {
  vector<vector<double>> matrix;
  for (int i = 0; i < n; i++) {
    std::vector<double> row(n, 0);
    row[i] = 1;
    matrix.emplace_back(row);
  }

  return matrix;
}

vector<vector<double>> v_vT_multiplication(vector<double> v) {
  vector<vector<double>> vvT;
  for (int i = 0; i < v.size(); i++) {
    vector<double> row;
    for (int j = 0; j < v.size(); j++) {
      row.push_back(v[i] * v[j]);
    }
    vvT.emplace_back(row);
  }

  return vvT;
}

void matrixMultiplication(const vector<vector<double>>& H,
                          vector<vector<double>>& A,
                          int t) {
  vector<vector<double>> R;
  int i, j, k;

  for (i = 0; i < H.size(); ++i) {
    R.emplace_back(vector<double>(A[0].size() - t, 0));
    for (j = 0; j < A[0].size() - t; ++j) {
      for (k = 0; k < H[0].size(); ++k) {
        R[i][j] += H[i][k] * A[k + t][j + t];
      }
      if (std::abs(R[i][j]) < 10e-8) {
        R[i][j] = 0;
      }
    }
  }

  std::cout << "--------" << t << "--------\n";
  // displayMatrix(R);

  for (int i = 0; i < R.size(); i++) {
    for (int j = 0; j < R[i].size(); j++) {
      A[i + t][j + t] = R[i][j];
    }
  }
  displayMatrix(A);
}

void householderReflection(vector<vector<double>>& matrix) {
  displayMatrix(matrix);
  vector<double> x;
  vector<vector<double>> I, vvT;

  for (int k = 0; k < matrix.size(); k++) {
    x = getColumn(matrix, k);
    displayVector("x:", x);
    double x_norm = euclideanNorm(x);
    std::cout << "x_norm: " << x_norm << "\n";
    if (!x_norm)
      continue;

    vector<double> unit_vector(x.size(), 0);
    vector<double> v(x.size(), 0);

    double x1 = matrix[k][k];
    double sign = (x1 > 0) ? 1 : -1;

    std::cout << "sign: " << sign << "\n";
    unit_vector[0] = sign * x_norm;
    displayVector("Unit vector:", unit_vector);

    std::transform(x.begin(), x.end(), unit_vector.begin(), v.begin(),
                   std::plus<double>());

    displayVector("v:", v);
    double v_norm = euclideanNorm(v);
    std::cout << "v_norm: " << v_norm << "\n";

    std::transform(v.begin(), v.end(), v.begin(),
                   [&v_norm](auto& c) { return c / v_norm; });

    vvT = v_vT_multiplication(v);

    for (int i = 0; i < vvT.size(); i++) {
      std::transform(vvT[i].begin(), vvT[i].end(), vvT[i].begin(),
                     [](auto& c) { return c * -2; });
    }

    I = identityMatrix(x.size());
    for (int i = 0; i < vvT.size(); i++) {
      std::transform(vvT[i].begin(), vvT[i].end(), I[i].begin(), vvT[i].begin(),
                     std::plus<double>());
    }

    // displayMatrix(vvT);

    matrixMultiplication(vvT, matrix, k);
  }

  displayMatrix(matrix);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "Command line format:\n\t."
                 "/<binary> <rows> <columns> <number_of_openMP_threads>\n\t"
                 "For example: ./HouseHolder.exe 1024 1024 4\n";
  } else {
    vector<vector<double>> matrix;
    int rows = std::stoi(argv[1]);
    int cols = std::stoi(argv[1]);
    generateRandomMatrix(matrix, rows, cols);
    // displayMatrix(matrix);
    householderReflection(matrix);
  }

  return 0;
}