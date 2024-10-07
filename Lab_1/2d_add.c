#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SIZE 10000

int A[SIZE][SIZE];
int B[SIZE][SIZE];
int C[SIZE][SIZE];

void initialize_matrices()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            A[i][j] = rand();
            B[i][j] = rand();
        }
    }
}

void serial_add()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void parallel_add(int total_threads)
{
    #pragma omp parallel for num_threads(total_threads)
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

int main(void)
{
    initialize_matrices();

    // Serial
    double start_time = omp_get_wtime();
    serial_add();
    double end_time = omp_get_wtime();
    double serial_time = end_time - start_time;

    printf("Serial time: %lf seconds\n", serial_time);

    // 2 threads
    start_time = omp_get_wtime();
    parallel_add(2);
    end_time = omp_get_wtime();
    double time_2_threads = end_time - start_time;

    // 4 threads
    start_time = omp_get_wtime();
    parallel_add(4);
    end_time = omp_get_wtime();
    double time_4_threads = end_time - start_time;

    // 8 threads
    start_time = omp_get_wtime();
    parallel_add(8);
    end_time = omp_get_wtime();
    double time_8_threads = end_time - start_time;

    printf("2 threads: %lf seconds\n", time_2_threads);
    printf("4 threads: %lf seconds\n", time_4_threads);
    printf("8 threads: %lf seconds\n", time_8_threads);

    return 0;
}
