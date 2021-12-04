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
                int64_t *A = NULL;
                A = (int64_t *) malloc(N * N * sizeof(int64_t));
                if(A == NULL){
                    fprintf(stderr, "[ERROR] Failed to allocate memory for matrix A");
                    return MALLOC_ERROR;
                }

                for(int i = 0; i < N * N; i++){
                    A[i] = rand() % 10;
                }

                printf("[INFO] Writing generated matrix to file 'a'\n");
                FILE *bin_m = fopen("a", "wb");
                fwrite(&N, sizeof(int), 1, bin_m);
                fwrite(A, sizeof(int64_t), N * N, bin_m);

                fclose(bin_m);
                printf("[INFO] File 'a' is ready!\n");

                free(A);
                break;
            case 'v':
            case 'V':
                printf("[INFO] Generating Vector of %d elements...\n", N);
                int64_t *b = (int64_t *) malloc(N * sizeof(int64_t));
                if(b == NULL){
                    fprintf(stderr, "[ERROR] Failed to allocate memory for vector b\n");
                    return MALLOC_ERROR;
                }

                printf("[INFO] Generating result(empty) vector\n");
                int64_t *c = (int64_t *) malloc(N * sizeof(int64_t));
                if(c == NULL){
                    fprintf(stderr, "[ERROR] Failed to allocate memory for vector b\n");
                    return MALLOC_ERROR;
                }

                for(int i = 0; i < N; i++){
                    b[i] = i;
                    c[i] = 0;
                }

                printf("[INFO] Writing generated vector to file 'b'\n");
                FILE *bin_v;
                bin_v = fopen("b", "wb");

                fwrite(b, sizeof(int64_t), N, bin_v);

                fclose(bin_v);
                printf("[INFO] File 'b' is ready!\n");

                printf("[INFO] Writing result(empty) vector to file 'c'\n");
                bin_v = fopen("c", "wb");
                fwrite(c, sizeof(int64_t), N, bin_v);
                fclose(bin_v);

                printf("[INFO] File 'c' is ready!\n");
                printf("\n");

                free(b);
                free(c);
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

