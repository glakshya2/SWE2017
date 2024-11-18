#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void parallel_prefix_sum(double *transactions, double *result, int n) {
    int num_threads;
    double *prefix_sum;

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        int start = id * (n / total_threads);
        int end = (id == total_threads - 1) ? n : start + (n / total_threads);

        result[start] = transactions[start];
        for (int i = start + 1; i < end; i++) {
            result[i] = result[i - 1] + transactions[i];
        }

        #pragma omp barrier

        #pragma omp single
        {
            prefix_sum = malloc(total_threads * sizeof(double));
            prefix_sum[0] = result[end - 1];
            for (int i = 1; i < total_threads; i++) {
                int prev_end = i * (n / total_threads) - 1;
                int curr_end = (i + 1) * (n / total_threads) - 1;
                prefix_sum[i] = prefix_sum[i - 1] + result[prev_end];
            }
        }

        #pragma omp barrier

        if (id > 0) {
            double offset = prefix_sum[id - 1];
            for (int i = start; i < end; i++) {
                result[i] += offset;
            }
        }
    }
    free(prefix_sum);
}

int main() {
    int n = 1000000;
    double *transactions = malloc(n * sizeof(double));
    double *running_total = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) {
        transactions[i] = (double)(rand() % 100);
    }

    parallel_prefix_sum(transactions, running_total, n);

    for (int i = 0; i < 10; i++) {
        printf("Transaction %d: %f, Running Total: %f\n", i, transactions[i], running_total[i]);
    }

    free(transactions);
    free(running_total);
    return 0;
}
