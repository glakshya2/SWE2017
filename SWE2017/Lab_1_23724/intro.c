#include <stdio.h>
#include <omp.h>

void DisplayMessage();

int main() {
    # pragma omp parallel num_threads(5)
    DisplayMessage();

    return 0;
}

void DisplayMessage() {
    int tn;
    tn = omp_get_thread_num();
    printf("Welcome to Parallel Programming lab %d\n", tn);
}