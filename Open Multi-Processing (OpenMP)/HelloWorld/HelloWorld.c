#include <stdio.h>
#include <omp.h>

int main(){

    //none clause. ID+1 -> indexing from 1
#pragma omp parallel default(none)
    {
        int ID = omp_get_thread_num();
        printf("Hello world from thread: %d\n", ID + 1);
    }
    return 0;
}

