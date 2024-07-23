#include <stdio.h>
#include <omp.h>

int a[] = {1, 2, 3, 4, 5};
int b[] = {6, 7, 8, 9, 10};
int c[5];


int main() {
    # pragma omp parallel num_threads(5)
    {
        int tid = omp_get_thread_num();
        c[tid] = a[tid] + b[tid];
    }

    for (int i = 0; i < 5; i++) {
        printf("%d\n", c[i]);
    }
}