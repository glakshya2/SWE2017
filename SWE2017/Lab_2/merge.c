#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void parallelMerge(int* A, int* B, int* mergedList, int n, int m, int num_threads) {
    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        int total_size = n + m;
        int chunk_size = (total_size + num_threads - 1) / num_threads;

        int start = thread_id * chunk_size;
        int end = start + chunk_size > total_size ? total_size : start + chunk_size;

        int a_start = 0, b_start = 0, k = start;

        // Find the starting point in A and B for this chunk
        for (int i = 0; i < start; i++) {
            if (a_start < n && (b_start >= m || A[a_start] <= B[b_start])) {
                a_start++;
            } else {
                b_start++;
            }
        }

        while (k < end) {
            if (a_start < n && (b_start >= m || A[a_start] <= B[b_start])) {
                mergedList[k++] = A[a_start++];
            } else {
                mergedList[k++] = B[b_start++];
            }
        }
    }
}

int main() {
    int A[] = {1, 4, 7, 10, 13, 16};
    int B[] = {2, 3, 8, 9, 14, 15};
    int n = sizeof(A) / sizeof(A[0]);
    int m = sizeof(B) / sizeof(B[0]);
    int num_threads = 4;

    int* mergedList = (int*)malloc((n + m) * sizeof(int));

    parallelMerge(A, B, mergedList, n, m, num_threads);

    for (int i = 0; i < n + m; i++) {
        printf("%d ", mergedList[i]);
    }
    printf("\n");

    free(mergedList);
    return 0;
}
