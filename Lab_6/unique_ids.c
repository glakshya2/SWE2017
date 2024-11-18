#include <stdio.h>
#include <omp.h>

int is_valid_combination(int digits[6]) {
    if (digits[0] == 0) return 0;

    for (int i = 1; i < 6; i++) {
        if (digits[i] == digits[i - 1]) return 0;
    }

    int sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += digits[i];
    }
    if (sum == 7 || sum == 11 || sum == 13) return 0;

    return 1;
}

int main() {
    int count = 0;

    #pragma omp parallel for reduction(+:count) collapse(6)
    for (int d1 = 1; d1 <= 9; d1++) {
        for (int d2 = 0; d2 <= 9; d2++) {
            if (d2 == d1) continue;
            for (int d3 = 0; d3 <= 9; d3++) {
                if (d3 == d2) continue;
                for (int d4 = 0; d4 <= 9; d4++) {
                    if (d4 == d3) continue;
                    for (int d5 = 0; d5 <= 9; d5++) {
                        if (d5 == d4) continue;
                        for (int d6 = 0; d6 <= 9; d6++) {
                            if (d6 == d5) continue;

                            int digits[6] = {d1, d2, d3, d4, d5, d6};

                            if (is_valid_combination(digits)) {
                                count++;
                            }
                        }
                    }
                }
            }
        }
    }

    printf("Total valid combinations: %d\n", count);
    return 0;
}
