//Reduction example 2
//Addition of numbers from 0 to 100
//Factorial of 15

#include <cmath>
#include <iostream>
#include <omp.h>

int main(){
    int sum = 0;
    long factorial = 1;
    /*
     * The reduction clause performs a reduction on the variables that appear in its list.
     * A private copy for each list variable is created for each thread. At the end of the reduction,
     * the reduction variable is applied to all private copies of the shared variable,
     * and the final result is written to the global shared variable.
     *
     * For variables specified in the reduction clause, they must satisfy the following conditions:
        Must be of a type appropriate to the operator. If the max or min operator is specified, the variables must be one of the following types with or without long, short, signed, or unsigned:
        _Bool (C only)
        bool (C++ only)
        char
        wchar_t (C++ only)
        int
        float
        double
        Must be shared in the enclosing context.
        Must not be const-qualified.
        Must not have pointer type.
     */

    omp_set_num_threads(256);
#pragma omp parallel for reduction(+:sum)
        for(int i =0; i<100; i++){
            sum += i;
        }

#pragma omp parallel for reduction(*:factorial)
        for(int i = 1; i<=15; i++){
            factorial *= i;
        }
    std::cout << "Sum of numbers from 0 to 10 = " << sum << "\n";
    std::cout << "Factorial of 15 = " << factorial << "\n";
    return 0;
}