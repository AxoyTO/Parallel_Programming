#include "mpi.h"
#include <stdio.h>
#define N 65536

int main(int argc, char **argv){

	int rank, n_proc, right, left, buf[N];
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n_proc);


	right = (rank+1 == n_proc) ? 0 : rank + 1;
	left = (rank-1 < 0 ) ? n_proc -1 : rank -1;

	MPI_Send(buf, N, MPI_INT, right, 0, MPI_COMM_WORLD);
	MPI_Recv(buf, N, MPI_INT, left, 0, MPI_COMM_WORLD, &stat);

	printf("Rank %d sent message to rank %d\n", rank, right);
	printf("Rank %d received message from rank %d\n", rank, left);

	MPI_Finalize();
}
