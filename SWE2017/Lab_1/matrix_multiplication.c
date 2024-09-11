#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define ROWS_A 500
#define COLS_A 1000
#define ROWS_B 1000
#define COLS_B 500

int A[ROWS_A][COLS_A];
int B[ROWS_B][COLS_B];
int C[ROWS_A][COLS_B];

void initialize_matrices()
{
    for (int i = 0; i < ROWS_A; i++)
        for (int j = 0; j < COLS_A; j++)
            A[i][j] = rand() % 100;

    for (int i = 0; i < COLS_A; i++)
        for (int j = 0; j < COLS_B; j++)
            B[i][j] = rand() % 100;
}

void serial_multiply()
{
    for (int i = 0; i < ROWS_A; i++)
    {
        for (int j = 0; j < COLS_B; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < COLS_A; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void parallel_multiply(int total_threads)
{
    #pragma omp parallel for num_threads(total_threads) collapse(2)
    for (int i = 0; i < ROWS_A; i++)
    {
        for (int j = 0; j < COLS_B; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < COLS_A; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(void)
{
    initialize_matrices();

    // Serial
    double start_time = omp_get_wtime();
    serial_multiply();
    double end_time = omp_get_wtime();
    double serial_time = end_time - start_time;
    printf("Serial time: %lf seconds\n", serial_time);

    // 2 threads
    start_time = omp_get_wtime();
    parallel_multiply(2);
    end_time = omp_get_wtime();
    double time_2_threads = end_time - start_time;
    printf("2 threads: %lf seconds\n", time_2_threads);

    // 4 threads
    start_time = omp_get_wtime();
    parallel_multiply(4);
    end_time = omp_get_wtime();
    double time_4_threads = end_time - start_time;
    printf("4 threads: %lf seconds\n", time_4_threads);

    // 8 threads
    start_time = omp_get_wtime();
    parallel_multiply(8);
    end_time = omp_get_wtime();
    double time_8_threads = end_time - start_time;
    printf("8 threads: %lf seconds\n", time_8_threads);

    return 0;
}
