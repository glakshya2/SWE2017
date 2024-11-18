#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

const int N = 1024;
int p;

void multiply_submatrices(double *A, double *B, double *C, int subSize) {
    for (int i = 0; i < subSize; ++i) {
        for (int j = 0; j < subSize; ++j) {
            for (int k = 0; k < subSize; ++k) {
                C[i * subSize + j] += A[i * subSize + k] * B[k * subSize + j];
            }
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    p = static_cast<int>(std::sqrt(size));
    int subSize = N / p;

    int dims[2] = {p, p};
    int periods[2] = {1, 1};
    MPI_Comm cartComm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cartComm);

    int coords[2];
    MPI_Cart_coords(cartComm, rank, 2, coords);

    std::vector<double> A_local(subSize * subSize, 0);
    std::vector<double> B_local(subSize * subSize, 0);
    std::vector<double> C_local(subSize * subSize, 0);

    MPI_Status status;
    for (int iter = 0; iter < p; ++iter) {
        multiply_submatrices(A_local.data(), B_local.data(), C_local.data(), subSize);

        int src, dst;
        MPI_Cart_shift(cartComm, 1, -1, &src, &dst);
        MPI_Sendrecv_replace(A_local.data(), subSize * subSize, MPI_DOUBLE, dst, 0, src, 0, cartComm, &status);

        MPI_Cart_shift(cartComm, 0, -1, &src, &dst);
        MPI_Sendrecv_replace(B_local.data(), subSize * subSize, MPI_DOUBLE, dst, 0, src, 0, cartComm, &status);
    }

    if (rank == 0) {
        std::vector<double> C(N * N, 0);
        MPI_Gather(C_local.data(), subSize * subSize, MPI_DOUBLE, C.data(), subSize * subSize, MPI_DOUBLE, 0, cartComm);
    } else {
        MPI_Gather(C_local.data(), subSize * subSize, MPI_DOUBLE, NULL, subSize * subSize, MPI_DOUBLE, 0, cartComm);
    }

    MPI_Finalize();
    return 0;
}
