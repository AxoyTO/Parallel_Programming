#include <iostream>
#include <omp.h>

int main() {
    int arr[1000], x = 10;
    /*
     * #pragma omp parallel default(none)
     * default(none) means that the variables won't be shared by default,
     * The variables inside the worksharing space will be unknown if they are not shared -> will lead to ERROR
     */

    /*
     * this won't give any errors because we specify the shared and private variables manually
     * so threads in the worksharing space will know about them
     */
#pragma omp parallel default(none) private(x) shared(arr, std::cout) num_threads(4)
    {
        x = 1;
        arr[0] = 2;
        std::cout << "[INSIDE] x: " << x << " and arr[0]: " << arr[0] << "\n";
    }
    std::cout << "[OUTSIDE] x: " << x << " and arr[0]: " << arr[0] << "\n";
    return 0;
}