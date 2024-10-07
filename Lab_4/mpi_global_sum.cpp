#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000000;
    std::vector<int> array(N);
    long local_sum = 0, global_sum = 0;
    int elements_per_proc = N / size;
    int remainder = N % size;

    if (rank == 0) {
        std::srand(std::time(0));
        for (int i = 0; i < N; ++i) {
            array[i] = std::rand() % 100;
        }
    }

    int local_size = (rank < remainder) ? elements_per_proc + 1 : elements_per_proc;
    std::vector<int> local_array(local_size);

    std::vector<int> sendcounts(size), displs(size);
    int offset = 0;
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = (i < remainder) ? elements_per_proc + 1 : elements_per_proc;
        displs[i] = offset;
        offset += sendcounts[i];
    }
    MPI_Scatterv(array.data(), sendcounts.data(), displs.data(), MPI_INT, local_array.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_size; ++i) {
        local_sum += local_array[i];
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Global sum: " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
