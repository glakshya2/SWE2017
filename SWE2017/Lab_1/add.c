#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#define SIZE 100000000


int a[SIZE];
int b[SIZE];
int c[SIZE];

void parallel_add()
{
    int tid = omp_get_thread_num();
    int total_threads = omp_get_num_threads();
    int start_pos = tid * (SIZE / total_threads);
    int end_pos = start_pos + (SIZE / total_threads);
    for (long long i = start_pos; i < end_pos; i++)
    {
        c[i] = a[i] + b[i];
    }
}

int main(void)
{

    for (long long i = 0; i < SIZE; i++)
    {
        a[i] = rand();
        b[i] = rand();
    }

    // Serially
    double start_time = omp_get_wtime();
    for (long long i = 0; i < SIZE; i++)
    {
        c[i] = a[i] + b[i];
    }
    double end_time = omp_get_wtime();
    double serial_time = end_time - start_time;

    // 2 threads
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(2)
    {
       parallel_add();
    }
    end_time = omp_get_wtime();
    double two_threads = end_time - start_time;

    // 4 threads
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(4)
    {
       parallel_add();
    }
    end_time = omp_get_wtime();
    double four_threads = end_time - start_time;

    // 6 threads
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(6)
    {
       parallel_add();
    }
    end_time = omp_get_wtime();
    double six_threads = end_time - start_time;

    // 8 threads
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(8)
    {
       parallel_add();
    }
    end_time = omp_get_wtime();
    double eight_threads = end_time - start_time;

    // 10 threads
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(10)
    {
       parallel_add();
    }
    end_time = omp_get_wtime();
    double ten_threads = end_time - start_time;


    printf("Serial time: %lf\n", serial_time);
    printf("2 threads: %lf\n", two_threads);
    printf("4 threads: %lf\n", four_threads);
    printf("6 threads: %lf\n", six_threads);
    printf("8 threads: %lf\n", eight_threads);
    printf("10 threads: %lf\n", ten_threads);

    return 0;
}

