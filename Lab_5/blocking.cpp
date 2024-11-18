#include <mpi.h>
#include <vector>

const int N = 1024;
const int P = 4;
const int blockSize = N / P;

void multiply_block(double* A, double* B, double* C, int size) {
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k)
                C[i * size + j] += A[i * size + k] * B[k * size + j];
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double A_local[blockSize * blockSize];
    double B_local[blockSize * blockSize];
    double C_local[blockSize * blockSize] = {0};

    MPI_Scatter(NULL, blockSize * blockSize, MPI_DOUBLE, A_local, blockSize * blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(NULL, blockSize * blockSize, MPI_DOUBLE, B_local, blockSize * blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int p = 0; p < P; ++p) {
        multiply_block(A_local, B_local, C_local, blockSize);
        MPI_Sendrecv_replace(B_local, blockSize * blockSize, MPI_DOUBLE, (rank + 1) % P, 0, (rank - 1 + P) % P, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Gather(C_local, blockSize * blockSize, MPI_DOUBLE, NULL, blockSize * blockSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
    