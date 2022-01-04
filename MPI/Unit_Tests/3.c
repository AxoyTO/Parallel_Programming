#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv){

	int local_num, n_proc;
	int global_num = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &local_num);
	MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

	MPI_Reduce(&local_num, &global_num, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	printf("global_num = %d\n", global_num);


	MPI_Finalize();
}
