#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(){
    int rank, comm_size;
    int random = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    srand(time(NULL));

    int broadcaster = rand() % comm_size;
    if(rank == broadcaster){
        random = rand() % 100;
    }
    MPI_Bcast(&random, 1, MPI_INT, broadcaster, MPI_COMM_WORLD);
    if(rank == broadcaster){
        printf("P:%d is broadcasting %d\n", rank, random);
    }
    //MPI_Barrier(MPI_COMM_WORLD);
    printf("P:%d received %d\n", rank, random);


    MPI_Finalize();
    return 0;
}
