#include <stdio.h>
#include <omp.h>

int main(void) {
    printf("Hello World\n");
    #pragma omp parallel num_threads(2)
    {
        printf("Hello World\n");
    }
    #pragma omp parallel num_threads(4)
    {
        printf("Hello World\n");
    }
    #pragma omp parallel num_threads(6)
    {
        printf("Hello World\n");
    }
    #pragma omp parallel num_threads(8)
    {
        printf("Hello World\n");
    }
    #pragma omp parallel num_threads(10)
    {
        printf("Hello World\n");
    }
}