#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp task shared(arr)
        mergeSort(arr, left, mid);
        
        #pragma omp task shared(arr)
        mergeSort(arr, mid + 1, right);

        #pragma omp taskwait
        merge(arr, left, mid, right);
    }
}

int main() {
    int n = 1000000; // Array size
    int* arr = (int*)malloc(n * sizeof(int));

    // Fill the array with random integers
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }

    double start_time = omp_get_wtime(); // Start timing
    #pragma omp parallel
    {
        #pragma omp single
        mergeSort(arr, 0, n - 1);
    }
    double end_time = omp_get_wtime(); // End timing

    printf("OpenMP Time: %f seconds\n", end_time - start_time);

    free(arr);
    return 0;
}
