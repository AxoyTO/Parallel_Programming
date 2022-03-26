#include "functions.hpp"
#include <map>

void generate_random_matrix(Matrix& init_A,
                            Matrix& A,
                            const int N,
                            int rank,
                            int comm_size) {
  allocate_matrix(init_A, N, rank, comm_size);
  allocate_matrix(A, N, rank, comm_size);

  for (int i = 0; i < A.size(); i++) {
    for (int j = 0; j < N; j++) {
      if (A[i].rank == j) {
        init_A[i].col[j] = 1.0 / double(1 + A[i].rank + j) + 0.1;
        A[i].col[j] = 1.0 / double(1 + A[i].rank + j) + 0.1;
      } else {
        init_A[i].col[j] = 1.0 / double(1 + A[i].rank + j);
        A[i].col[j] = 1.0 / double(1 + A[i].rank + j);
      }
    }
  }
}

void generate_random_vector(Vector& init_b,
                            Vector& b,
                            const Matrix& A,
                            const int N) {
  for (int j = 0; j < N; j++) {
    double sum = 0;
    for (int i = 0; i < A.size(); i++)
      if (A[i].rank % 2)
        sum += A[i].col[j];
    double res;
    MPI_Allreduce(&sum, &res, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    init_b[j] = res;
    b[j] = res;
  }
}

bool cmpA(Column a, Column b) {
  return a.rank < b.rank;
}

void print_matrix(Matrix A, int N, int rank, int comm_size) {
  if (rank == 0) {
    std::cout << "==============\n";
    for (int j = 1; j < comm_size; j++) {
      int ARec;
      MPI_Recv(&ARec, 1, MPI_INT, j, MPI_ANY_TAG, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

      for (int k = 0; k < ARec; k++) {
        Column tmp;
        tmp.col = (double*)malloc(N * sizeof(double));
        MPI_Recv(tmp.col, N, MPI_DOUBLE, j, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Recv(&(tmp.rank), 1, MPI_INT, j, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        A.push_back(tmp);
      }
    }

    std::sort(A.begin(), A.end(), cmpA);
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < A.size(); j++)
        if (fabs(A[j].col[i]) < EPS)
          std::cout << 0 << " ";
        else
          std::cout << A[j].col[i] << " ";
      std::cout << std::endl;
    }
    std::cout << "==============\n";
  } else {
    int size = A.size();
    MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    for (int i = 0; i < size; i++) {
      MPI_Send(A[i].col, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&(A[i].rank), 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
}

double euclideanNorm(const Vector& vec) {
  double norm = 0.0;
  for (const auto& v : vec) {
    norm += v * v;
  }
  return std::sqrt(norm);
}

void householder_reflection(Matrix& A,
                            Vector& b,
                            int N,
                            int rank,
                            int comm_size) {
  int p_num = 0;  // Process
  int cntColumn = 0;

  for (int i = 0; i < N - 1; ++i) {
    if (p_num == rank) {
      // Составить x
      double S = 0.0;
      for (int j = i + 1; j < N; ++j)
        S += A[cntColumn].col[j] * A[cntColumn].col[j];

      double a = sqrt(S + A[cntColumn].col[i] * A[cntColumn].col[i]);

      double* x;
      x = (double*)malloc(N * sizeof(double));
      memset(x, 0, N * sizeof(double));

      x[i] = A[cntColumn].col[i] - a;
      for (int j = i + 1; j < N; ++j)
        x[j] = A[cntColumn].col[j];

      double normX = sqrt(S + x[i] * x[i]);

      if (fabs(normX) > EPS)
        for (int j = 0; j < N; ++j)
          x[j] /= normX;

      MPI_Bcast(x, N, MPI_DOUBLE, p_num, MPI_COMM_WORLD);

      // Произвести перемножение
      for (int j = 0; j < A.size(); j++) {
        double sum = 0;
        for (int k = 0; k < N; ++k)
          sum += x[k] * A[j].col[k];

        for (int k = 0; k < N; k++)
          A[j].col[k] -= 2 * x[k] * sum;
      }

      double sum = 0;
      for (int j = 0; j < N; j++)
        sum += x[j] * b[j];

      for (int j = 0; j < N; j++)
        b[j] -= 2 * sum * x[j];

      free(x);
      cntColumn++;
    } else {
      double* x;
      x = (double*)malloc(N * sizeof(double));
      memset(x, 0, N * sizeof(double));

      MPI_Bcast(x, N, MPI_DOUBLE, p_num, MPI_COMM_WORLD);

      for (int j = 0; j < A.size(); j++) {
        double sum = 0;
        for (int k = 0; k < N; ++k)
          sum += x[k] * A[j].col[k];

        for (int k = 0; k < N; k++)
          A[j].col[k] -= 2 * x[k] * sum;
      }

      double sum = 0;
      for (int j = 0; j < N; j++)
        sum += x[j] * b[j];

      for (int j = 0; j < N; j++)
        b[j] -= 2 * sum * x[j];

      free(x);
    }

    p_num = (p_num + 1) % comm_size;
  }
}

void reverse_gaussian(Matrix& A,
                      Vector& b,
                      std::vector<std::pair<int, double>>& res,
                      int N,
                      int rank,
                      int comm_size) {
  int cntStep = N - 1;
  int cntColumn = A.size() - 1;
  int receiver = cntStep % comm_size;

  while (cntStep >= 0) {
    if ((cntColumn >= 0) && (A[cntColumn].rank == cntStep)) {
      double x = b[cntStep];
      for (int k = 0; k < res.size(); k++)
        x -= res[k].second * A[res[k].first / comm_size].col[cntStep];

      double tmpX;
      MPI_Reduce(&x, &tmpX, 1, MPI_DOUBLE, MPI_SUM, receiver, MPI_COMM_WORLD);
      x = tmpX;

      x /= A[cntColumn].col[cntStep];

      res.push_back(std::pair<int, double>(cntStep, x));
      cntColumn--;
    } else if (res.size()) {
      double sendX = 0;
      for (int k = 0; k < res.size(); k++)
        sendX += res[k].second * A[res[k].first / comm_size].col[cntStep];
      sendX *= -1;

      double tmpSendX;
      MPI_Reduce(&sendX, &tmpSendX, 1, MPI_DOUBLE, MPI_SUM, receiver,
                 MPI_COMM_WORLD);
    } else {
      double null = 0.0;
      double tmpNull = 0.0;
      MPI_Reduce(&null, &tmpNull, 1, MPI_DOUBLE, MPI_SUM, receiver,
                 MPI_COMM_WORLD);
    }

    cntStep--;
    receiver = cntStep % comm_size;
  }
}

void gatherResults(std::vector<std::pair<int, double>>& res,
                   int N,
                   int rank,
                   int comm_size) {
  if (rank == 0) {
    for (int i = 1; i < comm_size; ++i) {
      int cntX;
      MPI_Recv(&cntX, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      for (int j = 0; j < cntX; ++j) {
        int numx;
        double x;
        MPI_Recv(&numx, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Recv(&x, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        res.push_back(std::pair<int, double>(numx, x));
      }
    }
  } else {
    int size = res.size();
    MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    for (int i = 0; i < res.size(); i++) {
      MPI_Send(&(res[i].first), 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&(res[i].second), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
  }
}

double residual(Matrix& init_A,
                Vector& init_b,
                std::vector<std::pair<int, double>>& res,
                int N,
                int rank,
                int comm_size) {
  std::map<int, double> resMap;
  for (int i = 0; i < res.size(); ++i)
    resMap[res[i].first] = res[i].second;

  if (rank == 0) {
    double* tmp = (double*)malloc(N * sizeof(double));
    memset(tmp, 0, N * sizeof(double));
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < init_A.size(); j++)
        tmp[i] += init_A[j].col[i] * resMap[init_A[j].rank];
    }

    double* tmp2 = (double*)malloc(N * sizeof(double));
    MPI_Reduce(tmp, tmp2, N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double res = 0.0;
    for (int i = 0; i < N; i++)
      res += (tmp2[i] - init_b[i]) * (tmp2[i] - init_b[i]);
    res = sqrt(res);
    free(tmp);
    free(tmp2);
    return res;
  } else {
    double* tmp = (double*)malloc(N * sizeof(double));
    memset(tmp, 0, N * sizeof(double));
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < init_A.size(); j++)
        tmp[i] += init_A[j].col[i] * resMap[init_A[j].rank];
    }

    MPI_Reduce(tmp, NULL, N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    free(tmp);
  }

  return 0;
}

void allocate_matrix(Matrix& A, int N, int rank, int comm_size) {
  int size = N / comm_size + (rank < (N % comm_size) ? 1 : 0);
  A.resize(size);

  int col_rank = rank;
  for (int i = 0; i < size; i++) {
    A[i].col = (double*)malloc(N * sizeof(double));
    memset(A[i].col, 0, N * sizeof(double));
    A[i].rank = col_rank;
    col_rank += comm_size;
  }
}

void free_matrix(std::vector<Column>& A) {
  for (int i = 0; i < A.size(); i++)
    free(A[i].col);
}

void print_results(const int comm_size,
                   const int N,
                   const double t,
                   const double T1,
                   const double T2) {
  std::cout << "MPI Processes: " << comm_size << "\n";
  std::cout << "Matrix size: " << N << "\n";
  std::cout << "Residual: " << t << "\n";
  std::cout << "Householder Reflection elapsed time(T1): " << T1 << "\n";
  std::cout << "Reverse Gaussian elapsed time(T2): " << T2 << "\n";
}