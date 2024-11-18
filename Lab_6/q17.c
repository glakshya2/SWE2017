#include <stdio.h>

#define N 16

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void bitonicMerge(int arr[], int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        for (int i = low; i < low + k; i++) {
            if (dir == (arr[i] > arr[i + k])) {
                swap(&arr[i], &arr[i + k]);
            }
        }
        bitonicMerge(arr, low, k, dir);
        bitonicMerge(arr, low + k, k, dir);
    }
}

void bitonicSort(int arr[], int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        
        bitonicSort(arr, low, k, 1);

        bitonicSort(arr, low + k, k, 0);

        bitonicMerge(arr, low, cnt, dir);
    }
}

void sort(int arr[], int n, int up) {
    bitonicSort(arr, 0, n, up);
}

int main() {
    int arr[N] = {23, 45, 12, 67, 89, 34, 78, 10, 99, 88, 76, 55, 44, 32, 21, 18};

    printf("Original array:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    sort(arr, N, 1);

    printf("Sorted array:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
