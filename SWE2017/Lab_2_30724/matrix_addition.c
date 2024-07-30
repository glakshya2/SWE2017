#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdint.h>
// #define MAX_ARR 

int main(void) {
	int arr1[][SIZE_MAX] = {{1, 2, 3}, {4, 5, 6}};
	int arr2[][SIZE_MAX] = {{7, 8, 9}, {10, 11, 12}};
	int sum[2][SIZE_MAX];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < SIZE_MAX; j++) {
			arr1[i][j] = j;
			arr2[i][j] = j;
		}
	}	
	    clock_t t; 
    t = clock(); 
	#pragma omp parallel for num_threads(3)
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < SIZE_MAX; j++) {
			sum[i][j] = arr1[i][j] + arr2[i][j];
		}
	}
	  t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
 
    printf("parallel took %f seconds to execute \n", time_taken); 
	// for (int i = 0; i < 2; i++) {
	// 	for (int j = 0; j < 500; j++) {
	// 		printf("%d ", sum[i][j]);
	// 	}
	// 	printf("\n");
	// }

	 t = clock(); 
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < SIZE_MAX; j++) {
			sum[i][j] = arr1[i][j] + arr2[i][j];
		}
	}
	  t = clock() - t; 
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
	printf("serial took %f seconds to execute \n", time_taken);
}