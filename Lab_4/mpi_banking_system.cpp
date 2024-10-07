#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int global_balance = 10000;
    int local_balance = 0;
    int num_transactions = 5;

    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            int branch_balance;
            MPI_Recv(&branch_balance, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_balance += branch_balance;
        }

        for (int i = 1; i < size; ++i) {
            MPI_Send(&global_balance, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }

        std::cout << "Central server final global balance: " << global_balance << std::endl;
    } else {
        for (int i = 0; i < num_transactions; ++i) {
            int transaction = (i % 2 == 0) ? 100 : -50;
            local_balance += transaction;
        }

        MPI_Send(&local_balance, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(&global_balance, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Branch " << rank << " final global balance: " << global_balance << std::endl;
    }

    MPI_Finalize();
    return 0;
}