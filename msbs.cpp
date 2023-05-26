#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Bubble Sort - Sequential
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

//Bubble Sort - Parallel
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    int i, j;
    bool swapped;

    #pragma omp parallel default(none) shared(arr, n) private(i, j, swapped)
    {
        for (i = 0; i < n - 1; i++) {
            swapped = false;

            #pragma omp for
            for (j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                    swapped = true;
                }
            }

            #pragma omp barrier

            if (!swapped) {
                break;
            }
        }
    }
}

// Merging 2 subarrays
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> leftArr(n1);
    vector<int> rightArr(n2);

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Merge Sort - Sequential
void mergeSortSequential(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Merge Sort - Parallel
void mergeSortParallel(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, left, mid);
            #pragma omp section
            mergeSortParallel(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    int n = 20000;
    vector<int> arr(n);

    // Generate random array
    srand(time(0));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;

    vector<int> arrSeqBubble = arr;
    vector<int> arrParBubble = arr;
    vector<int> arrSeqMerge = arr;
    vector<int> arrParMerge = arr;

    // Measure sequential bubble sort performance
    auto startSeqBubble = high_resolution_clock::now();
    bubbleSortSequential(arrSeqBubble);
    auto stopSeqBubble = high_resolution_clock::now();
    auto durationSeqBubble = duration_cast<milliseconds>(stopSeqBubble - startSeqBubble);
    cout << "Sequential Bubble Sort: " << durationSeqBubble.count() << "ms" << endl;

    // Measure parallel bubble sort performance
    auto startParBubble = high_resolution_clock::now();
    bubbleSortParallel(arrParBubble);
    auto stopParBubble = high_resolution_clock::now();
    auto durationParBubble = duration_cast<milliseconds>(stopParBubble - startParBubble);
    cout << "Parallel Bubble Sort: " << durationParBubble.count() << "ms" << endl;

    // Measure sequential merge sort performance
    auto startSeqMerge = high_resolution_clock::now();
    mergeSortSequential(arrSeqMerge, 0, n - 1);
    auto stopSeqMerge = high_resolution_clock::now();
    auto durationSeqMerge = duration_cast<milliseconds>(stopSeqMerge - startSeqMerge);
    cout << "Sequential Merge Sort: " << durationSeqMerge.count() << "ms" << endl;

    // Measure parallel merge sort performance
    auto startParMerge = high_resolution_clock::now();
    mergeSortParallel(arrParMerge, 0, n - 1);
    auto stopParMerge = high_resolution_clock::now();
    auto durationParMerge = duration_cast<milliseconds>(stopParMerge - startParMerge);
    cout << "Parallel Merge Sort: " << durationParMerge.count() << "ms" << endl;

    return 0;
}
