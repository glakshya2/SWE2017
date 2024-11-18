#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int main(int argc, char** argv) {
    int rank, size;
    int limit = 1000000;
    int local_count = 0;
    int global_count;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int start = rank * (limit / size) + 1;
    if (start % 2 == 0) start++;
    int end = (rank + 1) * (limit / size);

    int prev_prime = -1;
    for (int i = start; i <= end; i += 2) {
        if (is_prime(i)) {
            if (prev_prime != -1 && i - prev_prime == 2) {
                local_count++;
            }
            prev_prime = i;
        }
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total number of consecutive odd prime pairs less than %d: %d\n", limit, global_count);
    }

    MPI_Finalize();

    return 0;
}
