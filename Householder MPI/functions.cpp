#include "functions.hpp"
#include <map>

#define RANDOM_GEN 1

double random_number_generator() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 10);

  return dist(rng);
}

void generate_random_matrix(Matrix& init_A,
                            Matrix& A,
                            const int N,
                            int rank,
                            int comm_size) {
  allocate_matrix(init_A, N, rank, comm_size);
  allocate_matrix(A, N, rank, comm_size);

  for (int i = 0; i < A.size(); i++) {
    for (int j = 0; j < N; j++) {
#if RANDOM_GEN
      double num = random_number_generator();
#else
      double num = 1.0 / double(1 + A[i].rank + j) + 0.1;
      // double num = 1;
#endif
      if (A[i].rank == j) {
        init_A[i].col[j] = num;
        A[i].col[j] = num;
      } else {
        init_A[i].col[j] = num;
        A[i].col[j] = num;
      }
    }
  }
}

void generate_random_vector(Vector& init_b,
                            Vector& b,
                            const Matrix& A,
                            const int N) {
  for (int j = 0; j < N; j++) {
#if RANDOM_GEN
    double sum = random_number_generator();
#else
    double sum = 0;
    for (int i = 0; i < A.size(); i++)
      if (A[i].rank % 2) {
        sum += A[i].col[j];
      }
#endif
    double res;
    MPI_Allreduce(&sum, &res, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    init_b[j] = res;
    b[j] = res;
  }
}

bool sort_compare(Column a, Column b) {
  return a.rank < b.rank;
}

void print_matrix(Matrix A, const int N, int rank, int comm_size) {
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

    std::sort(A.begin(), A.end(), sort_compare);
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < A.size(); j++)
        if (std::abs(A[j].col[i]) < 10e-7)
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

void print_vector(Vector& b, int rank) {
  if (rank == 0) {
    std::cout << "Vector: ";
    for (const auto& v : b) {
      std::cout << v << " ";
    }
    std::cout << "\n==============\n";
  }
}

/*
double euclidean_norm(const Vector& vec) {
  double norm = 0.0;
  for (const auto& v : vec) {
    norm += v * v;
  }
  return std::sqrt(norm);
}
*/

void householder_reflection(Matrix& A,
                            Vector& b,
                            const int N,
                            int rank,
                            int comm_size) {
  int p_num = 0;                     // Process  O(1)
  int col_num = 0;                   // A column   O(1)
  for (int i = 0; i < N - 1; i++) {  // O(N-1)
    // std::cout << "rank is " << rank << " -- upper p_num " << p_num << "\n";
    if (p_num == rank) {
      // std::cout << "upper rank" << rank << "\n";
      //   std::cout << "upper cycle p_num is " << p_num << "\n";
      //    First rank 0
      double S = 0.0;                  // For each rank const
      for (int j = i + 1; j < N; j++)  // For each rank const*N
        S += A[col_num].col[j] * A[col_num].col[j];

      double a = sqrt(S + A[col_num].col[i] *
                              A[col_num].col[i]);  // sqrt time complexity ?
                                                   // Probably no more than O(n)

      double* x;                                // O(1)
      x = (double*)malloc(N * sizeof(double));  // Non-deterministic
      memset(x, 0, N * sizeof(double));         // For each rank O(n)

      x[i] = A[col_num].col[i] - a;    // O(1)
      for (int j = i + 1; j < N; j++)  // For each rank const*N
        x[j] = A[col_num].col[j];

      double x_norm = sqrt(S + x[i] * x[i]);  // O(1)

      if (std::abs(x_norm) > 10e-7)
        for (int j = 0; j < N; j++)  // For each rank const*N
          x[j] /= x_norm;

      // std::cout << "above bcast\n";
      MPI_Bcast(
          x, N, MPI_DOUBLE, p_num,
          MPI_COMM_WORLD);  // MPI_Bcast ? O(N) ? send N times to P processors ?

      // matmult
      // O(N^2)
      for (int j = 0; j < A.size(); j++) {  // A.size = N ---> Finally O(N^2)
        double sum = 0;
        for (int k = 0; k < N; k++)  // O(N)
          sum += x[k] * A[j].col[k];

        for (int k = 0; k < N; k++)  // O(N)
          A[j].col[k] -= 2 * x[k] * sum;
      }

      double sum = 0;
      for (int j = 0; j < N; j++)  // O(N)
        sum += x[j] * b[j];

      for (int j = 0; j < N; j++)  // O(N)
        b[j] -= 2 * sum * x[j];

      free(x);
      col_num++;
    } else {  // --> Finally O(2N^2)
      // std::cout << "lower rank" << rank << "\n";
      double* x;                                // O(1)
      x = (double*)malloc(N * sizeof(double));  // Non-deterministic
      memset(x, 0, N * sizeof(double));         // O(N)

      // std::cout << "pnum is " << p_num << "\n";
      MPI_Bcast(x, N, MPI_DOUBLE, p_num, MPI_COMM_WORLD);  // O ?

      for (int j = 0; j < A.size(); j++) {  // A.size = N --> Finally O(2N^2)
        double sum = 0;
        for (int k = 0; k < N; k++)  // O(N)
          sum += x[k] * A[j].col[k];

        for (int k = 0; k < N; k++)  // O(N)
          A[j].col[k] -= 2 * x[k] * sum;
      }

      double sum = 0;
      for (int j = 0; j < N; j++)  // O(N)
        sum += x[j] * b[j];

      for (int j = 0; j < N; j++)  // O(N)
        b[j] -= 2 * sum * x[j];

      free(x);
    }

    p_num = (p_num + 1) % comm_size;  // O(1)
    // std::cout << "below p_num " << p_num << "\n";
  }
}

void reverse_gaussian(Matrix& A,
                      Vector& b,
                      std::vector<std::pair<int, double>>& res,
                      const int N,
                      int rank,
                      int comm_size) {
  int step = N - 1;  // Step
  int col_num = A.size() - 1;
  // std::cout << A.size() << "\n";
  int p_receiver = step % comm_size;

  while (step >= 0) {                                   // N
    if ((col_num >= 0) && (A[col_num].rank == step)) {  // N
      // std::cout << "rank " << rank << " is inside if -- step:" << step <<
      // "\n";
      double x = b[step];

      for (int k = 0; k < res.size(); k++) {  // ??
        // std::cout << "rank:" << rank << " step:" << step
        //           << " res.size:" << res.size() << "\n";
        x -= res[k].second * A[res[k].first / comm_size].col[step];
      }

      double x_temp;
      MPI_Reduce(&x, &x_temp, 1, MPI_DOUBLE, MPI_SUM, p_receiver,
                 MPI_COMM_WORLD);
      x = x_temp;

      x /= A[col_num].col[step];

      res.push_back(std::pair<int, double>(step, x));

      col_num--;
    } else if (res.size()) {
      // std::cout << "rank " << rank << " is inside elseif -- step" << step
      //           << "\n";
      double x_send = 0;
      for (int k = 0; k < res.size(); k++)
        x_send += res[k].second * A[res[k].first / comm_size].col[step];
      x_send *= -1;

      double x_temp_send;
      MPI_Reduce(&x_send, &x_temp_send, 1, MPI_DOUBLE, MPI_SUM, p_receiver,
                 MPI_COMM_WORLD);
    } else {
      // std::cout << "rank " << rank << " is inside else -- step" << step <<
      // "\n";
      double null = 0.0;
      double tmpNull = 0.0;
      MPI_Reduce(&null, &tmpNull, 1, MPI_DOUBLE, MPI_SUM, p_receiver,
                 MPI_COMM_WORLD);
    }

    step--;
    p_receiver = step % comm_size;
  }

  // std::cout << "rank: " << rank << " ";
  // std::cout << "rank -> " << rank << " res.size -> " << res.size() << "\n";
  // for (const auto& v : res) {
  //   std::cout << "first: " << v.first << " --- second: " << v.second << "\n";
  // }
}

void gather_results(std::vector<std::pair<int, double>>& res,
                    const int N,
                    int rank,
                    int comm_size) {
  if (rank == 0) {
    for (int i = 1; i < comm_size; i++) {
      int x_row;
      // std::cout << "rank: " << rank << " x_row:" << x_row << "\n";
      MPI_Recv(&x_row, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      for (int j = 0; j < x_row; j++) {
        int x_num;
        double x;
        MPI_Recv(&x_num, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Recv(&x, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        res.push_back(std::pair<int, double>(x_num, x));
      }
    }
  } else {
    int size = res.size();
    MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // std::cout << "rank:" << rank << " sends size:" << size << " to rank 0\n";
    for (int i = 0; i < res.size(); i++) {
      MPI_Send(&(res[i].first), 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&(res[i].second), 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      // std::cout << "rank:" << rank << " sends x_num:" << res[i].first
      //           << " to rank 0\n"
      //         << "rank:" << rank << " sends x:" << res[i].second
      //         << " to rank 0\n";
    }
  }
}

double norm_of_residual_vector(Matrix& init_A,
                               Vector& init_b,
                               std::vector<std::pair<int, double>>& res,
                               const int N,
                               int rank,
                               int comm_size) {
  std::map<int, double> result_map;
  for (int i = 0; i < res.size(); i++) {
    // std::cout << "res[" << i << "].first is " << res[i].first << "\n";
    result_map[res[i].first] = res[i].second;
  }

  if (rank == 0) {
    double* tmp = (double*)malloc(N * sizeof(double));
    memset(tmp, 0, N * sizeof(double));
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < init_A.size(); j++)
        tmp[i] += init_A[j].col[i] * result_map[init_A[j].rank];
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
        tmp[i] += init_A[j].col[i] * result_map[init_A[j].rank];
    }

    MPI_Reduce(tmp, NULL, N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    free(tmp);
  }

  return 0;
}

void allocate_matrix(Matrix& A, const int N, int rank, int comm_size) {
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
                   const double residual_norm,
                   const double T1,
                   const double T2) {
  // std::cout << "MPI Processes: " << comm_size << "\n";
  // std::cout << "Matrix size: " << N << "\n";
  std::cout << "Norm of residual vector: " << residual_norm << "\n";

  std::cout << "Householder Reflection elapsed time(T1): " << T1 << " s.\n";
  std::cout << "Reverse Gaussian elapsed time(T2): " << T2 << " s.\n";
  std::cout << "Total time elapsed(T1+T2): " << T1 + T2 << " s.\n";
}
