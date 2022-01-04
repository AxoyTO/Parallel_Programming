#include <iostream>
#include <omp.h>
#include <unistd.h>

void displayArray(int a[], int size) {
    for(int i = 0; i < size; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    int size = 10;
    auto A = new int[size];
    auto B = new int[size];
    auto C = new int[size];

    for(int i = 0; i < size; i++) {
        A[i] = i + 1;
        B[i] = (i + 1) * 2;
        C[i] = 0;
    }

    std::cout << "Array A: ";
    displayArray(A, size);
    std::cout << "Array B: ";
    displayArray(B, size);
    std::cout << "Array C: ";
    displayArray(C, size);

    double start, end;

    start = omp_get_wtime();
    int i;
#pragma omp parallel for schedule(static) default(none) private(i) shared(A, B, C, size, std::cout)
    for(i = 0; i < size; i++) {
        C[i] = A[i] + B[i];
    }
    end = omp_get_wtime();
    std::cout << "\n";
    std::cout << "Array A: ";
    displayArray(A, size);
    std::cout << "Array B: ";
    displayArray(B, size);
    std::cout << "Array C: ";
    displayArray(C, size);

    std::cout << "Elapsed Time: " << end - start << "\n";

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}