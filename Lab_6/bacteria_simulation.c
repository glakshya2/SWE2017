#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_COLONIES 1000
#define ITERATIONS 100
#define INITIAL_POPULATION 10

double random_growth_rate() {
    return 1.0 + ((double)rand() / RAND_MAX);
}

int main() {
    int colony_populations[NUM_COLONIES];
    double growth_rates[NUM_COLONIES];
    long total_population = 0;

    for (int i = 0; i < NUM_COLONIES; i++) {
        colony_populations[i] = INITIAL_POPULATION;
        growth_rates[i] = random_growth_rate();
    }

    for (int iter = 0; iter < ITERATIONS; iter++) {
        #pragma omp parallel for
        for (int i = 0; i < NUM_COLONIES; i++) {
            colony_populations[i] *= growth_rates[i];

            growth_rates[i] = random_growth_rate();
        }

        long local_total = 0;
        #pragma omp parallel for reduction(+:local_total)
        for (int i = 0; i < NUM_COLONIES; i++) {
            local_total += colony_populations[i];
        }
        total_population = local_total;

        printf("Iteration %d: Total Population = %ld\n", iter + 1, total_population);
    }

    return 0;
}
