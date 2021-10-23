/*
 * False sharing occurs when two or more threads' uses a shared memory
 * and the memory is stored in the same cache line. The result of program will not be affected
 * but the code may work way slower than expected if false sharing occurs. To fix this,
 * 'padding' can be used to divide memory which different threads should reach into different cache lines.
 *
 * (Size of cache) = (Cache Lines) * (Size of each cache Line)
 * For example:
 *  My processor's(AMD Ryzen 5 3600 - 6 cores):
 *      L2 cache has a line size of 64 bytes.
 *      Has 512KB per core. So -> 512KB = (x cache lines) * 64 bytes
 *      Each core in my processor has 8192 cache lines.
 */

#include <omp.h>
#include <iostream>

int main(){
    int *x = new int[20];
    omp_set_num_threads(20);
    int in_sum = 0, out_sum = 0;
    //x and sum are shared by all threads
    //num of threads are 20
    //Specifying default(shared) is equivalent to stating each variable in a shared(list) clause.
    //Specifying default(none) requires that each data variable visible to the parallelized statement block must be explcitly listed in a data scope clause, with the exception of those variables that are:
    //-const-qualified,
    //-specified in an enclosed data scope attribute clause, or,
    //-used as a loop control variable referenced only by a corresponding omp for or omp parallel for directive.
#pragma omp parallel shared(x, in_sum, out_sum) default(none)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        int stride = 1;

        for(int k = 0; k < 200; k++){
            if(thread_id != 0 && thread_id != 19){
                x[thread_id * stride]++;
                in_sum++;
            }
            out_sum++;
        }
    }
    std::cout << "\n";
    for(int i = 0; i < 20; i++){
        std::cout << x[i] << " ";
    }
    std::cout << "\n";
    std::cout << in_sum << "\n";
    std::cout << out_sum << "\n";
    delete[]x;
    return 0;
}