#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long long compute_partial_product(int start, int end) {
    unsigned long long product = 1;
    for (int i = start; i <= end; i++) {
        product *= i;
    }
    return product;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int N = 10;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int numbers_per_process = N / size;
    int remainder = N % size;

    int start = rank * numbers_per_process + 1;
    int end = start + numbers_per_process - 1;

    if (rank < remainder) {
        start += rank;
        end += rank + 1;
    } else {
        start += remainder;
        end += remainder;
    }

    unsigned long long local_product = compute_partial_product(start, end);

    unsigned long long global_product = 1;
    MPI_Reduce(&local_product, &global_product, 1, MPI_UNSIGNED_LONG_LONG, MPI_PROD, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The factorial of %d is: %llu\n", N, global_product);
    }

    MPI_Finalize();
    return 0;
}
