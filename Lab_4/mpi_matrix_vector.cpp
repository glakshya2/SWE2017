#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 4;
    std::vector<int> A(N * N);
    std::vector<int> x(N);
    std::vector<int> y(N);

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            x[i] = i + 1;
            for (int j = 0; j < N; ++j) {
                A[i * N + j] = i + j + 1;
            }
        }
    }

    MPI_Bcast(x.data(), N, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_process = N / size;
    std::vector<int> local_A(rows_per_process * N);
    std::vector<int> local_y(rows_per_process);

    MPI_Scatter(A.data(), rows_per_process * N, MPI_INT, local_A.data(), rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_process; ++i) {
        local_y[i] = 0;
        for (int j = 0; j < N; ++j) {
            local_y[i] += local_A[i * N + j] * x[j];
        }
    }

    MPI_Gather(local_y.data(), rows_per_process, MPI_INT, y.data(), rows_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Final result vector y: ";
        for (int i = 0; i < N; ++i) {
            std::cout << y[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
