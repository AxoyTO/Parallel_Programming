#pragma once
#include "functions.hpp"

int check_matrix_singularity(const Matrix& columns,
                             const int N,
                             int rank,
                             int comm_size);

/*
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

*/