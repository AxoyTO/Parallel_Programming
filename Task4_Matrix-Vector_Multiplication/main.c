#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MALLOC_ERROR 1

int main(int argc, char **argv){
    if(argc > 3){
        int rank, comm_size;
        double start, end;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

        int64_t N;
        int64_t **M = NULL;
        int64_t *V = NULL, *res_V = NULL;
        int64_t local_N;

        FILE *f;
        f = fopen(argv[1], "rb");
        fread(&N, sizeof(int64_t), 1, f);
        M = (int64_t **) malloc(N * sizeof(int64_t *));
        if(M == NULL){
            fprintf(stderr, "[ERROR] Failed to allocate memory for (M)atrix\n");
            return MALLOC_ERROR;
        }
        for(int i = 0; i < N; i++){
            *(M + i) = (int64_t *) malloc(N * sizeof(int64_t));
        }
        for(int i = 0; i < N; i++){
            fread(*(M + i), sizeof(int64_t), N, f);
        }
        fclose(f);

        /*
        printf("%lu\n", N);
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                printf("%ld ", M[i][j]);
            }
            printf("\n");
        }*/
        f = fopen(argv[2], "rb");
        fread(&N, sizeof(int64_t), 1, f);
        V = (int64_t *) malloc(N * sizeof(int64_t));
        if(V == NULL){
            fprintf(stderr, "[ERROR] Failed to allocate memory for (V)ector\n");
            return MALLOC_ERROR;
        }
        fread(V, sizeof(int64_t), N, f);
        fclose(f);

        /*
        printf("%lu\n", N);
        for(int i = 0; i < N; i++){
            printf("%ld ", V[i]);
        }
        printf("\n");
        */


        f = fopen(argv[3], "rb");
        fread(&N, sizeof(int64_t), 1, f);
        res_V = (int64_t *) malloc(N * sizeof(int64_t));
        if(res_V == NULL){
            fprintf(stderr, "[ERROR] Failed to allocate memory for result vector\n");
            return MALLOC_ERROR;
        }
        fread(res_V, sizeof(int64_t), N, f);
        fclose(f);

        /*
        printf("%lu\n", N);
        for(int i = 0; i < N; i++){
            printf("%ld ", res_V[i]);
        }
        printf("\n");
        */

        local_N = N / comm_size;

        start = MPI_Wtime();

        //parallel_matrix_vector_multiplication(M, V, res_V,)

        end = MPI_Wtime();

        if(rank == 0){
            printf("PROCESSES: %d\n"
                   "ELAPSED TIME: %f\n", comm_size, end - start);
        }


        for(int i = 0; i < N; i++){
            free(M[i]);
        }
        free(M);
        free(V);
        free(res_V);
        MPI_Finalize();
    } else{
        fprintf(stderr, "Command line format:\n"
                        "\t./<binary> <matrix> <vector> <result_vector>\n");
    }
    return 0;
}

