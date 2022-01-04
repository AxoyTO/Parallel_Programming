#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int comm_size;
    int rank;
    char buf[50] = "Hello World! P:";
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0)
        printf("Total cores: %d\n", comm_size);
    MPI_Barrier(MPI_COMM_WORLD);

    printf("%s %d\n", buf, rank);

    MPI_Finalize();
    return 0;
}
