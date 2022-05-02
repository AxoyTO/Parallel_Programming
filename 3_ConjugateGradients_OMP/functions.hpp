#include <unordered_map>
#include <vector>

namespace CGSolver {

using std::unordered_map;
using std::vector;
using Vector = vector<double>;
using Matrix = vector<Vector>;
using Row_Col_Val_UOMap = unordered_map<int, std::pair<Vector, Vector>>;

void generate_cube_and_sparse_matrix(const int N_side, Matrix& sparse_matrix);

void generate_row_col_val(const Matrix& sparse_matrix,
                          Row_Col_Val_UOMap& row_col_val);

void generate_vector_b(Vector& vec);

void print_matrix(const Matrix& sparse_matrix);

void print_vector(const Vector& vec);

void print_row_col_val(const Row_Col_Val_UOMap& row_col_val);

void set_diagonal_matrix_M(const Matrix& A, Matrix& M);

void inverse_diagonal_matrix(Matrix& M);

// Preconditioning
Vector precondition(const Matrix& M, const Vector& r);
void dot_product();

// ax plus by
void axpby();

// Sparse Matrix-Vector Calculation
void spmv(const Matrix& matrix, const Vector& vec);

}  // namespace CGSolver