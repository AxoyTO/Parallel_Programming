#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    int thrNum; //номер нити
    int thrCount; //число нитей
    int size;
    int* v;
    int* sum; //переменная для суммирования
} pthrData;

void* threadFunc(void* thread_data){
    //получаем структуру с данными
    pthrData *data = (pthrData*) thread_data;
    //копируем переменные на стек функции
    int thrCount = data->thrCount;
    int thrNum = data->thrNum;
    int thread_sum = 0;
    int* v = data->v;

    for(int i = thrNum; i < size; i += thrCount) {
        threadSum += v[i];
    }
    
    pthread_mutex_lock(&mutex);//вход в критическую секцию
    *(data->sum) += thread_sum;
    pthread_mutex_unlock(&mutex);//освобождаем мьютекс

    return NULL;
}

int vector_sum(int *v, size, int nthreads) {
    //выделяем память под массив идентификаторов потоков
    pthread_t* threads = (pthread_t*) malloc(nthreads* sizeof(pthread_t));
    //сколько потоков - столько и структур с потоковых данных
    pthrData* threadData = (pthrData*) malloc(nthreads* sizeof(pthrData));
    int sum = 0;
    int i;
    for (i = 0; i < nthreads; ++i) {
        threadData[i].thrNum = i;
        threadData[i].thrCount = nthreads;
        threadData[i].size = size;
        threadData[i].v = v;
        threadData[i].sum = &sum;
        //запускаем поток
        if(pthread_create(&(threads[i]), NULL, threadFunc, threadData + i) != 0) {
            free(threads);
            free(threadData);
            return INT_MAX;
        }
    }
    for(int k = 0; k < i; ++k)
        pthread_join(threads[k], NULL);
    free(threads);
    free(threadData);
    return sum;
}
