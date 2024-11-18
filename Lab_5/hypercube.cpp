#include <mpi.h>
#include <iostream>
#include <vector>

const int N = 1024;
const int D = 4;

void multiply_submatrices(double* A, double* B, double* C, int subSize) {
    for (int i = 0; i < subSize; ++i)
        for (int j = 0; j < subSize; ++j)
            for (int k = 0; k < subSize; ++k)
                C[i * subSize + j] += A[i * subSize + k] * B[k * subSize + j];
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int p = 1 << D;
    int subSize = N / (1 << (D / 2));

    std::vector<double> A_local(subSize * subSize, 1.0);
    std::vector<double> B_local(subSize * subSize, 1.0);
    std::vector<double> C_local(subSize * subSize, 0.0);

    for (int dim = 0; dim < D; ++dim) {
        int partner = rank ^ (1 << dim);
        MPI_Sendrecv_replace(A_local.data(), subSize * subSize, MPI_DOUBLE, partner, 0, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv_replace(B_local.data(), subSize * subSize, MPI_DOUBLE, partner, 0, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        multiply_submatrices(A_local.data(), B_local.data(), C_local.data(), subSize);
    }

    MPI_Gather(C_local.data(), subSize * subSize, MPI_DOUBLE, NULL, subSize * subSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}
