#include <bits/stdc++.h>
#include <chrono>
#include <omp.h>
using namespace std;
using namespace std::chrono;

// ===================== BUBBLE SORT =====================

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        #pragma omp parallel for
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ===================== MERGE SORT =====================

// Merge function
vector<int> mergeVectors(vector<int>& left, vector<int>& right) {
    vector<int> result;
    int i = 0, j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            result.push_back(left[i++]);
        } else {
            result.push_back(right[j++]);
        }
    }

    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);

    return result;
}

// Sequential Merge Sort (FIXED)
vector<int> sequentialMergeSort(vector<int> arr) {
    if (arr.size() <= 1) return arr;

    int mid = arr.size() / 2;

    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    left = sequentialMergeSort(left);
    right = sequentialMergeSort(right);

    return mergeVectors(left, right);
}

// Parallel Merge Sort (FIXED + THRESHOLD)
vector<int> parallelMergeSort(vector<int> arr) {
    // Threshold to avoid too many threads
    if (arr.size() <= 1000)
        return sequentialMergeSort(arr);

    int mid = arr.size() / 2;

    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    #pragma omp parallel sections
    {
        #pragma omp section
        left = parallelMergeSort(left);

        #pragma omp section
        right = parallelMergeSort(right);
    }

    return mergeVectors(left, right);
}

// ===================== MAIN =====================

int main() {
    int n = 100000;

    // ---------------- BUBBLE SORT ----------------
    cout << "=== BUBBLE SORT ===\n";

    vector<int> arr1(n);
    for (int i = 0; i < n; i++) arr1[i] = rand() % 1000;

    auto start = high_resolution_clock::now();
    sequentialBubbleSort(arr1);
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end - start);
    cout << "Sequential: " << duration1.count() << " ms\n";

    vector<int> arr2(n);
    for (int i = 0; i < n; i++) arr2[i] = rand() % 1000;

    start = high_resolution_clock::now();
    parallelBubbleSort(arr2);
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end - start);
    cout << "Parallel:   " << duration2.count() << " ms\n";
    cout << "Speedup: " << (double)duration1.count() / duration2.count() << "x\n\n";

    // ---------------- MERGE SORT ----------------
    cout << "=== MERGE SORT ===\n";

    vector<int> arr3(n);
    for (int i = 0; i < n; i++) arr3[i] = rand() % 1000;

    start = high_resolution_clock::now();
    arr3 = sequentialMergeSort(arr3);
    end = high_resolution_clock::now();
    duration1 = duration_cast<milliseconds>(end - start);
    cout << "Sequential: " << duration1.count() << " ms\n";

    vector<int> arr4(n);
    for (int i = 0; i < n; i++) arr4[i] = rand() % 1000;

    start = high_resolution_clock::now();
    arr4 = parallelMergeSort(arr4);
    end = high_resolution_clock::now();
    duration2 = duration_cast<milliseconds>(end - start);
    cout << "Parallel:   " << duration2.count() << " ms\n";
    cout << "Speedup: " << (double)duration1.count() / duration2.count() << "x\n";

    return 0;
}