#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if(argc == 3){
        int32_t N = atoi(argv[1]);
        char T = *argv[2];
        if(T == 'd'){
            int32_t **A = NULL;
            int32_t **B = NULL;
            int32_t **C = NULL;
            printf("Type: int32_t\n");

            //Allocate memory for matrix A rows
            A = (int32_t **) malloc(N * sizeof(int32_t *));
            if(A == NULL){
                fprintf(stderr, "Error allocating memory for matrix A\n");
                return 1;
            }

            //Allocate memory for matrix A columns
            for(int i = 0; i < N; i++){
                *(A + i) = (int32_t *) malloc(N * sizeof(int32_t));
                if(*(A + i) == NULL){
                    fprintf(stderr, "Error allocating memory for matrix A[%d]\n", i);
                    return 1;
                }
            }

            //Allocate memory for matrix B rows
            B = (int32_t **) malloc(N * sizeof(int32_t *));
            if(B == NULL){
                fprintf(stderr, "Error allocating memory for matrix A\n");
                return 1;
            }

            //Allocate memory for matrix B columns
            for(int i = 0; i < N; i++){
                *(B + i) = (int32_t *) malloc(N * sizeof(int32_t));
                if(*(B + i) == NULL){
                    fprintf(stderr, "Error allocating memory for matrix A[%d]\n", i);
                    return 1;
                }
            }
            //Allocate memory for matrix C rows
            C = (int32_t **) malloc(N * sizeof(int32_t *));
            if(C == NULL){
                fprintf(stderr, "Error allocating memory for matrix A\n");
                return 1;
            }

            //Allocate memory for matrix C columns
            for(int i = 0; i < N; i++){
                *(C + i) = (int32_t *) malloc(N * sizeof(int32_t));
                if(*(C + i) == NULL){
                    fprintf(stderr, "Error allocating memory for matrix A[%d]\n", i);
                    return 1;
                }
            }

            //Initialize matrix A
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    A[i][j] = i + 1;
                    B[i][j] = j + 1;
                    C[i][j] = 0;
                }
            }

            //Write Matrix A to file a
            FILE *bin_a = fopen("a", "wb");
            fwrite(&T, sizeof(char), 1, bin_a);
            fwrite(&N, sizeof(int32_t), 1, bin_a);
            for(int i = 0; i < N; i++){
                fwrite(*(A + i), sizeof(int32_t), N, bin_a);
            }
            fclose(bin_a);

            //Write Matrix B to file b
            FILE *bin_b = fopen("b", "wb");
            fwrite(&T, sizeof(char), 1, bin_b);
            fwrite(&N, sizeof(int32_t), 1, bin_b);
            for(int i = 0; i < N; i++){
                fwrite(*(B + i), sizeof(int32_t), N, bin_b);
            }
            fclose(bin_b);


            //Write Matrix C to file c
            FILE *bin_c = fopen("c", "wb");
            fwrite(&T, sizeof(char), 1, bin_c);
            fwrite(&N, sizeof(int32_t), 1, bin_c);
            for(int i = 0; i < N; i++){
                fwrite(*(C + i), sizeof(int32_t), N, bin_c);
            }
            fclose(bin_c);

            for(int i = 0; i < N; i++){
                free(A[i]);
                free(B[i]);
                free(C[i]);
            }
            free(A);
            free(B);
            free(C);

        } else if(T == 'l'){
            int64_t **A = NULL;
            int64_t **B = NULL;
            int64_t **C = NULL;
            printf("Type: int64_t\n");

            //Allocate memory for matrix A rows
            A = (int64_t **) malloc(N * sizeof(int64_t *));
            if(A == NULL){
                fprintf(stderr, "Error allocating memory for matrix A\n");
                return 1;
            }

            //Allocate memory for matrix A columns
            for(int i = 0; i < N; i++){
                *(A + i) = (int64_t *) malloc(N * sizeof(int64_t));
                if(*(A + i) == NULL){
                    fprintf(stderr, "Error allocating memory for matrix A[%d]\n", i);
                    return 1;
                }
            }

            //Allocate memory for matrix B rows
            B = (int64_t **) malloc(N * sizeof(int64_t *));
            if(B == NULL){
                fprintf(stderr, "Error allocating memory for matrix A\n");
                return 1;
            }

            //Allocate memory for matrix B columns
            for(int i = 0; i < N; i++){
                *(B + i) = (int64_t *) malloc(N * sizeof(int64_t));
                if(*(B + i) == NULL){
                    fprintf(stderr, "Error allocating memory for matrix A[%d]\n", i);
                    return 1;
                }
            }
            //Allocate memory for matrix C rows
            C = (int64_t **) malloc(N * sizeof(int64_t *));
            if(C == NULL){
                fprintf(stderr, "Error allocating memory for matrix A\n");
                return 1;
            }

            //Allocate memory for matrix C columns
            for(int i = 0; i < N; i++){
                *(C + i) = (int64_t *) malloc(N * sizeof(int64_t));
                if(*(C + i) == NULL){
                    fprintf(stderr, "Error allocating memory for matrix A[%d]\n", i);
                    return 1;
                }
            }

            //Initialize matrix A
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    A[i][j] = i + 1;
                    B[i][j] = j + 1;
                    C[i][j] = 0;
                }
            }
            //Write Matrix A to file a
            FILE *bin_a = fopen("a", "wb");
            fwrite(&T, sizeof(char), 1, bin_a);
            fwrite(&N, sizeof(int32_t), 1, bin_a);
            for(int i = 0; i < N; i++){
                fwrite(*(A + i), sizeof(int64_t), N, bin_a);
            }
            fclose(bin_a);

            //Write Matrix B to file b
            FILE *bin_b = fopen("b", "wb");
            fwrite(&T, sizeof(char), 1, bin_b);
            fwrite(&N, sizeof(int32_t), 1, bin_b);
            for(int i = 0; i < N; i++){
                fwrite(*(B + i), sizeof(int64_t), N, bin_b);
            }
            fclose(bin_b);


            //Write Matrix C to file c
            FILE *bin_c = fopen("c", "wb");
            fwrite(&T, sizeof(char), 1, bin_c);
            fwrite(&N, sizeof(int32_t), 1, bin_c);
            for(int i = 0; i < N; i++){
                fwrite(*(C + i), sizeof(int64_t), N, bin_c);
            }
            fclose(bin_c);

            for(int i = 0; i < N; i++){
                free(A[i]);
                free(B[i]);
                free(C[i]);
            }
            free(A);
            free(B);
            free(C);

        } else{
            fprintf(stderr, "Type %c is not valid...\nType should be 'd' or 'l'\n", *argv[2]);
        }

    } else{
        fprintf(stderr,
                "Command line format:\n ./<binary> <N_of_square_matrix> <Type_of_elements>\n");
        fprintf(stderr,
                "Type should be d or l: d for int32_t and l for int64_t\n");
    }
    return 0;
}