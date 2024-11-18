#include <stdio.h>
#include <omp.h>

int main() {
    int total_transactions = 100;
    int transactions_per_thread = 20;
    int shared_counter = 0;

    #pragma omp parallel num_threads(5)
    {
        for (int i = 0; i < transactions_per_thread; i++) {
            #pragma omp critical
            {
                shared_counter++;
            }
        }
    }

    printf("Total processed transactions: %d\n", shared_counter);
    return 0;
}
