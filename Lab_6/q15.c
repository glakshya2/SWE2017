#include <stdio.h>
#include <omp.h>

#define SIZE 2

void matrix_multiplication_hypercube(int A[SIZE][SIZE], int B[SIZE][SIZE], int C[SIZE][SIZE]) {
    #pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();
        
        if (id == 0) {
            C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
        } else if (id == 1) {
            C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
        } else if (id == 2) {
            C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
        } else if (id == 3) {
            C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
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
    int A[SIZE][SIZE] = { {1, 2}, {3, 4} };
    int B[SIZE][SIZE] = { {5, 6}, {7, 8} };
    int C[SIZE][SIZE] = {0};

    matrix_multiplication_hypercube(A, B, C);

    printf("Result of Matrix Multiplication (A * B):\n");
    print_matrix(C);

    return 0;
}
