#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

using namespace std;

void mark_primes(vector<bool>& is_prime, long long lim) {
    is_prime[0] = is_prime[1] = false;
    for (long long i = 2; i <= sqrt(lim); i++) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= lim; j+= i) {
                is_prime[j] = false;
            }
        }
    }
}

void count_twins(int n) {
    const long long lim = 5000000;
    vector<bool> is_prime(lim + 1, true);
    mark_primes(is_prime, lim);
    long long count = 0;
    #pragma omp parallel for reduction(+:count) num_threads(n)
    for (long long i = 2; i < is_prime.size(); i++) {
        if (is_prime[i] && is_prime[i - 2]) {
            count++;
        }
    }
    cout << "Count: " << count << endl;
}

int main(void) {
    double start_time = omp_get_wtime();
    count_twins(1);
    double end_time = omp_get_wtime();
    cout << "1 thread Time: " << end_time - start_time << endl;
    start_time = omp_get_wtime();
    count_twins(2);
    end_time = omp_get_wtime();
    cout << "2 thread Time: " << end_time - start_time << endl;
    start_time = omp_get_wtime();
    count_twins(4);
    end_time = omp_get_wtime();
    cout << "4 thread Time: " << end_time - start_time << endl;
    start_time = omp_get_wtime();
    count_twins(8);
    end_time = omp_get_wtime();
    cout << "8 thread Time: " << end_time - start_time << endl;
}
