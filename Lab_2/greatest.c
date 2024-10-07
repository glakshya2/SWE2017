#include <stdio.h>
#include <omp.h>
#include <float.h>

int main() {
    int n = 3650;
    float temperatures[n];
    for (int i = 0; i < n; i++) {
        temperatures[i] = 15.0 + (i % 35); 
    }

    float max_temp = -FLT_MAX;
    double start_time = omp_get_wtime();

    // Parallel section to find max temperature
    #pragma omp parallel for reduction(max:max_temp)
    for (int i = 0; i < n; i++) {
        if (temperatures[i] > max_temp) {
            max_temp = temperatures[i];
        }
    }

    double end_time = omp_get_wtime();
    printf("Maximum temperature recorded: %.2fC\n", max_temp);
    printf("Parallel execution time: %.6f seconds\n", end_time - start_time);

    // Reset max_temp for sequential execution
    max_temp = -FLT_MAX;
    start_time = omp_get_wtime();

    // Sequential section to find max temperature
    for (int i = 0; i < n; i++) {
        if (temperatures[i] > max_temp) {
            max_temp = temperatures[i];
        }
    }

    end_time = omp_get_wtime();
    printf("Maximum temperature recorded: %.2fC\n", max_temp);
    printf("Sequential execution time: %.6f seconds\n", end_time - start_time);

    return 0;
}
