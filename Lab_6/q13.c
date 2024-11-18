#include <stdio.h>
#include <omp.h>

#define SIZE 4

void matrix_addition_row_oriented(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrix_addition_column_oriented(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]) {
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrix_addition_parallel(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]) {
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int A[SIZE][SIZE] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    int B[SIZE][SIZE] = {
        {16, 15, 14, 13},
        {12, 11, 10, 9},
        {8, 7, 6, 5},
        {4, 3, 2, 1}
    };

    int C[SIZE][SIZE];

    matrix_addition_row_oriented(A, B, C);
    printf("Result of Row-Oriented Matrix Addition:\n");
    print_matrix(C);

    matrix_addition_column_oriented(A, B, C);
    printf("\nResult of Column-Oriented Matrix Addition:\n");
    print_matrix(C);

    matrix_addition_parallel(A, B, C);
    printf("\nResult of Parallelized Matrix Addition:\n");
    print_matrix(C);

    return 0;
}
