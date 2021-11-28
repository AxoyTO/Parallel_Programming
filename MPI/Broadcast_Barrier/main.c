#include <stdio.h>
#include <mpi.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


time_t start, stop;

void start_timer(){
    start = clock();
}

void stop_timer(){
    stop = clock();
}

float get_timer(){
    stop_timer();
    float timer = (float) (stop - start) / CLOCKS_PER_SEC;
    return timer;
}


void debug(int rank, char *format, ...){
    // Variable argument list
    va_list args;

    //Initialize variable argument list ---> 'format' is last argument before '...'
    va_start(args, format);

    printf("%.6f secs | P:%2d | ", get_timer(), rank);
    vprintf(format, args);

    //Clean up variable argument list
    va_end(args);
}

int broadcast(int rank, int world_size){
    int random;
    int broadcaster_rank = 0;

    if(rank == broadcaster_rank){
        srand(time(NULL) + rank);
        random = rand() % 1000;
        debug(rank, "Broadcasting %d\n", random);
    }

    MPI_Bcast(&random, 1, MPI_INT, broadcaster_rank, MPI_COMM_WORLD);

    if(rank != broadcaster_rank)
        debug(rank, "Received %d\n", random);

    return random;
}

void barrier(int rank, int random){
    int nap_time = rank;
    debug(rank, "Sleeping %ds\n", nap_time);
    sleep(nap_time);

    debug(rank, "Entered barrier\n");
    MPI_Barrier(MPI_COMM_WORLD);
    debug(rank, "Left barrier\n");
}

int main(int argc, char **argv){
    int rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start_timer();

    debug(rank, "Hello from P:%d\n", rank);
    int random = broadcast(rank, world_size);
    barrier(rank, random);
    debug(rank, "Goodbye from P:%d\n", rank);

    MPI_Finalize();
    return 0;
}
