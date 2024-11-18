#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 32

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void hyperquicksort(int *data, int n, int rank, int num_procs, MPI_Comm comm) {
    if (num_procs == 1) {
        qsort(data, n, sizeof(int), compare);
        return;
    }

    int pivot;
    if (rank == 0) {
        pivot = data[n / 2];
    }
    MPI_Bcast(&pivot, 1, MPI_INT, 0, comm);

    int *left = (int*)malloc(n * sizeof(int));
    int *right = (int*)malloc(n * sizeof(int));
    int left_count = 0, right_count = 0;

    for (int i = 0; i < n; i++) {
        if (data[i] <= pivot) {
            left[left_count++] = data[i];
        } else {
            right[right_count++] = data[i];
        }
    }

    int partner = rank ^ (num_procs / 2);
    int send_count = (rank < partner) ? right_count : left_count;
    int recv_count;

    MPI_Sendrecv(&send_count, 1, MPI_INT, partner, 0, &recv_count, 1, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);

    int *recv_buf = (int*)malloc(recv_count * sizeof(int));
    if (rank < partner) {
        MPI_Sendrecv(right, right_count, MPI_INT, partner, 0, recv_buf, recv_count, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
    } else {
        MPI_Sendrecv(left, left_count, MPI_INT, partner, 0, recv_buf, recv_count, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
    }

    int new_size = (rank < partner) ? left_count + recv_count : right_count + recv_count;
    int *new_data = (int*)malloc(new_size * sizeof(int));
    if (rank < partner) {
        memcpy(new_data, left, left_count * sizeof(int));
        memcpy(new_data + left_count, recv_buf, recv_count * sizeof(int));
    } else {
        memcpy(new_data, right, right_count * sizeof(int));
        memcpy(new_data + right_count, recv_buf, recv_count * sizeof(int));
    }

    free(data);
    free(left);
    free(right);
    free(recv_buf);

    MPI_Comm new_comm;
    MPI_Comm_split(comm, rank < partner, rank, &new_comm);
    hyperquicksort(new_data, new_size, rank, num_procs / 2, new_comm);

    MPI_Comm_free(&new_comm);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int data[ARRAY_SIZE] = {34, 72, 13, 88, 21, 90, 45, 3, 56, 27, 67, 1, 99, 12, 38, 79, 40, 25, 61, 92, 17, 84, 19, 75, 8, 53, 22, 66, 49, 31, 5, 43};
    int local_size = ARRAY_SIZE / num_procs;
    int *local_data = (int*)malloc(local_size * sizeof(int));

    MPI_Scatter(data, local_size, MPI_INT, local_data, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    hyperquicksort(local_data, local_size, rank, num_procs, MPI_COMM_WORLD);

    MPI_Gather(local_data, local_size, MPI_INT, data, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sorted array:\n");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}
