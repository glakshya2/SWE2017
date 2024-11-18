#include <stdio.h>
#include <stdlib.h>

double** allocateMatrix(int rows, int cols) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(cols * sizeof(double));
    }
    return matrix;
}

void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void matrixAddition(double** A, double** B, double** result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}

void matrixMultiplication(double** A, double** B, double** result, int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

double sumOfElements(double** matrix, int rows, int cols) {
    double sum = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sum += matrix[i][j];
        }
    }
    return sum;
}

void displayMatrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int rowsA = 2, colsA = 3;
    int rowsB = 3, colsB = 2;
    
    double** A = allocateMatrix(rowsA, colsA);
    double** B = allocateMatrix(rowsB, colsB);
    double** C = allocateMatrix(rowsA, colsB);
    double** D = allocateMatrix(rowsA, colsA);
    
    A[0][0] = 1; A[0][1] = 2; A[0][2] = 3;
    A[1][0] = 4; A[1][1] = 5; A[1][2] = 6;
    
    B[0][0] = 7; B[0][1] = 8;
    B[1][0] = 9; B[1][1] = 10;
    B[2][0] = 11; B[2][1] = 12;
    
    matrixAddition(A, A, D, rowsA, colsA);
    printf("Matrix Addition (A + A):\n");
    displayMatrix(D, rowsA, colsA);

    matrixMultiplication(A, B, C, rowsA, colsA, colsB);
    printf("\nMatrix Multiplication (A * B):\n");
    displayMatrix(C, rowsA, colsB);

    double sum = sumOfElements(A, rowsA, colsA);
    printf("\nSum of elements in Matrix A: %.2f\n", sum);

    freeMatrix(A, rowsA);
    freeMatrix(B, rowsB);
    freeMatrix(C, rowsA);
    freeMatrix(D, rowsA);

    return 0;
}
