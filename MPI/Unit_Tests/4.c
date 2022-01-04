#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv){

	int rank, n_proc, buf1, buf2;
	int global_num = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

	switch(rank)
	{
		case 0:
			buf1=1;
			MPI_Bcast(&buf1, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(&buf2, 1, MPI_INT, 1, MPI_COMM_WORLD);
			break;
		case 1:
			buf2 = 2;
			MPI_Bcast(&buf2, 1, MPI_INT, 1, MPI_COMM_WORLD);
			MPI_Bcast(&buf1, 1, MPI_INT, 0, MPI_COMM_WORLD);
			break;	
	}

	
	printf("buf1 = %d\nbuf2 = %d\n", buf1, buf2);


	MPI_Finalize();
}
