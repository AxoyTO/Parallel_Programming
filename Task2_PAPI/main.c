#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <papi.h>

void mode0_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int i, j, k;
    int32_t sum;
    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
}

void mode0_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int i, j, k;
    int64_t sum;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void mode1_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int i, k, j;
    int32_t tmp;
    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}

void mode1_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int i, k, j;
    int64_t tmp;
    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}

void mode2_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int k, i, j;
    int32_t tmp;
    for(k = 0; k < N; k++){
        for(i = 0; i < N; i++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}

void mode2_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int k, i, j;
    int64_t tmp;
    for(k = 0; k < N; k++){
        for(i = 0; i < N; i++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}

void mode3_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int j, i, k;
    int32_t sum;
    for(j = 0; j < N; j++){
        for(i = 0; i < N; i++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void mode3_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int j, i, k;
    int64_t sum;
    for(j = 0; j < N; j++){
        for(i = 0; i < N; i++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void mode4_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int j, k, i;
    int32_t tmp;
    for(j = 0; j < N; j++){
        for(k = 0; k < N; k++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}

void mode4_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int j, k, i;
    int64_t tmp;
    for(j = 0; j < N; j++){
        for(k = 0; k < N; k++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}

void mode5_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int k, j, i;
    int32_t tmp;
    for(k = 0; k < N; k++){
        for(j = 0; j < N; j++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}

void mode5_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int k, j, i;
    int64_t tmp;
    for(k = 0; k < N; k++){
        for(j = 0; j < N; j++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}

void handle_error(int retval, int l, char *s){
    printf("PAPI error %d: %s at line: %d, %s\n", retval, PAPI_strerror(retval), l, s);
}

int main(int argc, char **argv){
    if(argc == 5){
        char T;
        int32_t N;
        int mode = atoi(argv[4]);
        FILE *bin_a, *bin_b, *bin_c;
        clock_t begin, end;
        double elapsed_time = 0;
        int32_t **A_32 = NULL;
        int64_t **A_64 = NULL;
        int32_t **B_32 = NULL;
        int64_t **B_64 = NULL;
        int32_t **C_32 = NULL;
        int64_t **C_64 = NULL;

        int ret_val;
        long long counters[4];
        int event_set = PAPI_NULL;

        ret_val = PAPI_library_init(PAPI_VER_CURRENT);
        if(ret_val != PAPI_VER_CURRENT){
            fprintf(stderr, "Failed to initialize PAPI\n");
            return 1;
        }

        ret_val = PAPI_create_eventset(&event_set);
        if(ret_val != PAPI_OK){
            handle_error(ret_val, __LINE__, __FILE__);
            return 2;
        }

        ret_val = PAPI_add_event(event_set, PAPI_L1_LDM);   //LOAD
        if(ret_val != PAPI_OK){
            handle_error(ret_val, __LINE__, __FILE__);
            return 3;
        }
        ret_val = PAPI_add_event(event_set, PAPI_L1_STM);   //STORE
        if(ret_val != PAPI_OK){
            handle_error(ret_val, __LINE__, __FILE__);
            return 3;
        }

        ret_val = PAPI_add_event(event_set, PAPI_L1_DCM);   //DATA CACHE MISS
        if(ret_val != PAPI_OK){
            handle_error(ret_val, __LINE__, __FILE__);
            return 3;
        }

        ret_val = PAPI_add_event(event_set, PAPI_L1_DCA);   //DATA CACHE ACCESS
        if(ret_val != PAPI_OK){
            handle_error(ret_val, __LINE__, __FILE__);
            return 3;
        }

        bin_a = fopen("a", "rb");
        fread(&T, sizeof(char), 1, bin_a);
        fread(&N, sizeof(int32_t), 1, bin_a);
        if(T == 'd'){
            A_32 = (int32_t **) malloc(N * sizeof(int32_t *));
            for(int i = 0; i < N; i++){
                *(A_32 + i) = (int32_t *) malloc(N * sizeof(int32_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(A_32 + i), sizeof(int32_t), N, bin_a);
            }
        } else if(T == 'l'){
            A_64 = (int64_t **) malloc(N * sizeof(int64_t *));
            for(int i = 0; i < N; i++){
                *(A_64 + i) = (int64_t *) malloc(N * sizeof(int64_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(A_64 + i), sizeof(int64_t), N, bin_a);
            }
        }

        fclose(bin_a);
        bin_b = fopen("b", "rb");
        fread(&T, sizeof(char), 1, bin_b);
        fread(&N, sizeof(int32_t), 1, bin_b);
        if(T == 'd'){
            B_32 = (int32_t **) malloc(N * sizeof(int32_t *));
            for(int i = 0; i < N; i++){
                *(B_32 + i) = (int32_t *) malloc(N * sizeof(int32_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(B_32 + i), sizeof(int32_t), N, bin_b);
            }
        } else if(T == 'l'){
            B_64 = (int64_t **) malloc(N * sizeof(int64_t *));
            for(int i = 0; i < N; i++){
                *(B_64 + i) = (int64_t *) malloc(N * sizeof(int64_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(B_64 + i), sizeof(int64_t), N, bin_b);
            }
        }
        fclose(bin_b);

        bin_c = fopen("c", "rb");
        fread(&T, sizeof(char), 1, bin_c);
        fread(&N, sizeof(int32_t), 1, bin_c);
        if(T == 'd'){
            C_32 = (int32_t **) malloc(N * sizeof(int32_t *));

            for(int i = 0; i < N; i++){
                *(C_32 + i) = (int32_t *) malloc(N * sizeof(int32_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(C_32 + i), sizeof(int32_t), N, bin_c);
            }
        } else if(T == 'l'){
            C_64 = (int64_t **) malloc(N * sizeof(int64_t *));
            for(int i = 0; i < N; i++){
                *(C_64 + i) = (int64_t *) malloc(N * sizeof(int64_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(C_64 + i), sizeof(int64_t), N, bin_c);
            }
        }
        fclose(bin_c);

        if(T == 'd'){
            switch(mode){
                case 0:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 4;
                    }
                    begin = clock();
                    mode0_32(N, A_32, B_32, C_32);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 5;
                    }
                    break;
                case 1:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 6;
                    }
                    begin = clock();
                    mode1_32(N, A_32, B_32, C_32);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 7;
                    }
                    break;
                case 2:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 8;
                    }
                    begin = clock();
                    mode2_32(N, A_32, B_32, C_32);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 9;
                    }
                    break;
                case 3:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 10;
                    }
                    begin = clock();
                    mode3_32(N, A_32, B_32, C_32);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 11;
                    }
                    break;
                case 4:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 12;
                    }
                    begin = clock();
                    mode4_32(N, A_32, B_32, C_32);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 13;
                    }
                    break;
                case 5:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 14;
                    }
                    begin = clock();
                    mode5_32(N, A_32, B_32, C_32);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 15;
                    }
                    break;
                default:
                    fprintf(stderr, "Wrong mode...\n");
                    break;
            }
            if(mode >= 0 && mode <= 5){
                elapsed_time = (double) (end - begin) / CLOCKS_PER_SEC;
                //LOAD, STORE, DATA CACHE MISS, DATA CACHE ACCESS
                printf("Mode: %d --- Elapsed time: %.3lf s. --- ", mode, elapsed_time);
                printf("L1 Load(LDM): %lld (%.4lf%%) | L1 Store(STM): %lld (%.4lf%%) | L1 Data Cache Miss(DCM): %lld (%.4lf%%)\n",
                       counters[0], (double) counters[0] / (double) counters[3],
                       counters[1], (double) counters[1] / (double) counters[3],
                       counters[2], (double) counters[2] / (double) counters[3]);
            }
        } else if(T == 'l'){
            switch(mode){
                case 0:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 16;
                    }
                    begin = clock();
                    mode0_64(N, A_64, B_64, C_64);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 17;
                    }
                    break;
                case 1:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 18;
                    }
                    begin = clock();
                    mode1_64(N, A_64, B_64, C_64);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 19;
                    }
                    break;
                case 2:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 20;
                    }
                    begin = clock();
                    mode2_64(N, A_64, B_64, C_64);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 21;
                    }
                    break;
                case 3:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 22;
                    }
                    begin = clock();
                    mode3_64(N, A_64, B_64, C_64);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 23;
                    }
                    break;
                case 4:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 24;
                    }
                    begin = clock();
                    mode4_64(N, A_64, B_64, C_64);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 25;
                    }
                    break;
                case 5:
                    ret_val = PAPI_start(event_set);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 26;
                    }
                    begin = clock();
                    mode5_64(N, A_64, B_64, C_64);
                    end = clock();
                    ret_val = PAPI_stop(event_set, counters);
                    if(ret_val != PAPI_OK){
                        handle_error(ret_val, __LINE__, __FILE__);
                        return 27;
                    }
                    break;
                default:
                    fprintf(stderr, "Wrong mode...\n");
                    break;
            }
            if(mode >= 0 && mode <= 5){
                elapsed_time = (double) (end - begin) / CLOCKS_PER_SEC;
                printf("Mode: %d --- Elapsed time: %.3lf s. --- ", mode, elapsed_time);
                printf("L1 Load(LDM): %lld (%.4lf%%) | L1 Store(STM): %lld (%.4lf%%) | L1 Data Cache Miss(DCM): %lld (%.4lf%%)\n",
                       counters[0], (double) counters[0] / (double) counters[3],
                       counters[1], (double) counters[1] / (double) counters[3],
                       counters[2], (double) counters[2] / (double) counters[3]);
            }
            //display_matrix_64(C_64, N);
        }

        if(T == 'd'){
            for(int i = 0; i < N; i++){
                free(A_32[i]);
                free(B_32[i]);
                free(C_32[i]);
            }
            free(A_32);
            free(B_32);
            free(C_32);
        } else if(T == 'l'){
            for(int i = 0; i < N; i++){
                free(A_64[i]);
                free(B_64[i]);
                free(C_64[i]);
            }
            free(A_64);
            free(B_64);
            free(C_64);
        }
        PAPI_shutdown();
    } else{
        fprintf(stderr,
                "Command line format:\n ./<binary> <matrix_a> <matrix_b> <matrix_c> <mode>\n");
        fprintf(stderr,
                "Mode can be 0,1,2,3,4,5:\n"
                "0 for ijk\n"
                "1 for ikj\n"
                "2 for kij\n"
                "3 for jik\n"
                "4 for jki\n"
                "5 for kji\n");
    }

    return 0;
}

