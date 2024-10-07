#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAX_SIZE 1000000

int main(void) {
    int A[MAX_SIZE];
    int B[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        A[i] = rand() % 100;
    }
    double start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < MAX_SIZE; i++) {
        B[i] = A[i] * A[i];
    }
    double end_time = omp_get_wtime();
    printf("Parallel time: %f\n", end_time - start_time);

    start_time = omp_get_wtime();
    for (int i = 0; i < MAX_SIZE; i++) {
        B[i] = A[i] * A[i];
    }
    end_time = omp_get_wtime();
    printf("Serial time: %f\n", end_time - start_time);
    return 0;
}