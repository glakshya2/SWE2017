#include <stdio.h>
#include <omp.h>

int main() {
    int n = 3650;
    float temperatures[n];
    for (int i = 0; i < n; i++) {
        temperatures[i] = 15.0 + (i % 35);
    }

    float sum_temp = 0.0f;

    // Parallel for loop to calculate the sum of temperatures
    #pragma omp parallel for reduction(+:sum_temp)
    for (int i = 0; i < n; i++) {
        sum_temp += temperatures[i];
    }

    printf("Sum of all temperatures: %.2f\n", sum_temp);

    return 0;
}
