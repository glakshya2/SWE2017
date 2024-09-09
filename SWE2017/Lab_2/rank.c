#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void serialListRanking(int* Next, int* Rank, int n) {
    int done = 0;
    while (!done) {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (Next[i] != -1) {
                Rank[i] += Rank[Next[i]];
                Next[i] = Next[Next[i]];
                done = 0;
            }
        }
    }
}

void parallelListRanking(int* Next, int* Rank, int n) {
    while (1) {
        int done = 1;
        #pragma omp parallel for reduction(&&:done)
        for (int i = 0; i < n; i++) {
            if (Next[i] != -1) {
                Rank[i] += Rank[Next[i]];
                Next[i] = Next[Next[i]];
                done = 0;
            }
        }
        if (done) {
            break;
        }
    }
}

int main() {
    int n = 10000000;
    int* largeArray = (int*)malloc(n * sizeof(int));
    int* Next = (int*)malloc(n * sizeof(int));
    int* RankSerial = (int*)malloc(n * sizeof(int));
    int* RankParallel = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        largeArray[i] = i + 1;
    }
    largeArray[n-1] = -1;
    for (int i = 0; i < n; ++i) {
        Next[i] = largeArray[i];
        RankSerial[i] = 1;
        RankParallel[i] = 1;
    }
    RankSerial[0] = 0;
    RankParallel[0] = 0;

    double start_time_serial = omp_get_wtime();
    serialListRanking(Next, RankSerial, n);
    double end_time_serial = omp_get_wtime();
    printf("Serial implementation execution time: %.6f seconds\n", end_time_serial - start_time_serial);

    for (int i = 0; i < n; ++i) {
        Next[i] = largeArray[i];
    }

    double start_time_parallel = omp_get_wtime();
    parallelListRanking(Next, RankParallel, n);
    double end_time_parallel = omp_get_wtime();
    printf("Parallel implementation execution time: %.6f seconds\n", end_time_parallel - start_time_parallel);

    free(largeArray);
    free(Next);
    free(RankSerial);
    free(RankParallel);

    return 0;
}
