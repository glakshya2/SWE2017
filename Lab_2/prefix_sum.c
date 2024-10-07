#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void parallel_prefix_sum(int* A, int* P, int n) {
    int num_threads;
    
    #pragma omp parallel
    {
        num_threads = omp_get_num_threads();
    }
    
    int* partial_sums = (int*) malloc(num_threads * sizeof(int));
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start = tid * n / num_threads;
        int end = (tid + 1) * n / num_threads;
        P[start] = A[start];
        
        for (int i = start + 1; i < end; i++) {
            P[i] = P[i - 1] + A[i];
        }
        
        partial_sums[tid] = P[end - 1];
    }
    
    for (int i = 1; i < num_threads; i++) {
        partial_sums[i] += partial_sums[i - 1];
    }
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start = tid * n / num_threads;
        int end = (tid + 1) * n / num_threads;

        if (tid > 0) {
            for (int i = start; i < end; i++) {
                P[i] += partial_sums[tid - 1];
            }
        }
    }
    
    free(partial_sums);
}

void sequential_prefix_sum(int* A, int* P, int n) {
    P[0] = A[0];        
    for (int i = 1; i < n; i++) {
        P[i] = P[i - 1] + A[i];
    }
}

int main() {
    int A[] = {2, 4, 6, 8, 10, 12, 14, 16};
    int n = sizeof(A) / sizeof(A[0]);
    int* P = (int*) malloc(n * sizeof(int));
    
    double start_time = omp_get_wtime();
    sequential_prefix_sum(A, P, n);
    double end_time = omp_get_wtime();

    printf("Serial time: %lf seconds\n", end_time - start_time);
    start_time = omp_get_wtime();
    parallel_prefix_sum(A, P, n);
    end_time = omp_get_wtime();
    printf("Parallel time: %lf seconds\n", end_time - start_time);

    printf("Prefix Sum Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", P[i]);
    }
    
    free(P);
    return 0;
}
