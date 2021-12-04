#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_ERROR 1

int main(int argc, char **argv){
    if(argc > 2){
        if(strlen(argv[1]) != 1){
            goto inputError;
        }
        char type = *argv[1];
        int N = atoi(argv[2]);
        if(N <= 0){
            goto inputError;
        }
        switch(type){
            case 'm':
            case 'M':
                printf("[INFO] Generating Matrix of %dx%d elements...\n", N, N);
                int64_t *M = NULL;
                M = (int64_t *) malloc(N * N * sizeof(int64_t));
                if(M == NULL){
                    fprintf(stderr, "[ERROR] Failed to allocate memory for matrix M");
                    return MALLOC_ERROR;
                }

                for(int i = 0; i < N * N; i++){
                    M[i] = rand() % 10;
                }

                printf("[INFO] Writing generated matrix to file 'matrix'\n");
                FILE *bin_m = fopen("matrix", "wb");
                fwrite(&N, sizeof(int), 1, bin_m);
                fwrite(M, sizeof(int64_t), N * N, bin_m);

                fclose(bin_m);
                printf("[INFO] File 'matrix' is ready!\n");

                free(M);
                break;
            case 'v':
            case 'V':
                printf("[INFO] Generating Vector of %d elements...\n", N);
                int64_t *V = (int64_t *) malloc(N * sizeof(int64_t));
                if(V == NULL){
                    fprintf(stderr, "[ERROR] Failed to allocate memory for vector V\n");
                    return MALLOC_ERROR;
                }

                printf("[INFO] Generating result(empty) vector\n");
                int64_t *result_V = (int64_t *) malloc(N * sizeof(int64_t));
                if(result_V == NULL){
                    fprintf(stderr, "[ERROR] Failed to allocate memory for vector V\n");
                    return MALLOC_ERROR;
                }

                for(int i = 0; i < N; i++){
                    V[i] = i;
                    result_V[i] = 0;
                }

                printf("[INFO] Writing generated vector to file 'vector'\n");
                FILE *bin_v;
                bin_v = fopen("vector", "wb");

                fwrite(V, sizeof(int64_t), N, bin_v);

                fclose(bin_v);
                printf("[INFO] File 'vector' is ready!\n");

                printf("[INFO] Writing result(empty) vector to file 'result_vector'\n");
                bin_v = fopen("result_vector", "wb");
                fwrite(result_V, sizeof(int64_t), N, bin_v);
                fclose(bin_v);

                printf("[INFO] File 'result_vector' is ready!\n");
                printf("\n");

                free(V);
                free(result_V);
                break;
            default:
                goto inputError;
        }
    } else{
        inputError:
        fprintf(stderr, "Command line format:\n\t./<binary> <type> <size_of_type>\n"
                        "Size(N) must be N > 0\n\n"
                        "Type must be (M|m) or (V|v):\n"
                        "\t -> (M|m) for Matrix\n"
                        "\t -> (V|v) for Vector\n");
    }

    return 0;
}

