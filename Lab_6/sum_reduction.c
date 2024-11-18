#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size, I = 1, p = 10;
    int local_value, sum;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_value = rank + I;

    MPI_Reduce(&local_value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sum of series using reduction: %d\n", sum);

        int expected_sum = p * (p + 1) / 2;
        printf("Expected sum using formula p(p + 1) / 2: %d\n", expected_sum);
    }

    MPI_Finalize();

    return 0;
}
