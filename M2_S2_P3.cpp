#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono;
using namespace std;

// Function to generate a random vector
void randomVector(int vector[], int start, int end) {
    for (int i = start; i < end; i++) {
        vector[i] = rand() % 100; // Random value between 0 and 99
    }
}

// Function to perform vector addition
void vectorAddition(int v1[], int v2[], int v3[], int start, int end) {
    for (int i = start; i < end; i++) {
        v3[i] = v1[i] + v2[i]; // Add corresponding elements
    }
}

int main() {
    unsigned long size = 100000000; // Size of the vectors
    srand(time(0)); // Seed the random number generator

    int *v1, *v2, *v3; // Pointers for the vectors
    v1 = (int *)malloc(size * sizeof(int)); // Allocate memory for v1
    v2 = (int *)malloc(size * sizeof(int)); // Allocate memory for v2
    v3 = (int *)malloc(size * sizeof(int)); // Allocate memory for v3

    // Get the number of available threads
    int num_threads = thread::hardware_concurrency();
    cout << "Number of threads: " << num_threads << endl;

    // Ask the user to specify the partition size
    int partition_size;
    cout << "Enter partition size (number of elements per thread): ";
    cin >> partition_size;

    // Calculate the number of partitions
    int num_partitions = size / partition_size;
    if (size % partition_size != 0) {
        num_partitions++; // Handle the last partition if size is not divisible
    }

    cout << "Number of partitions: " << num_partitions << endl;

    // Start timing the execution
    auto start = high_resolution_clock::now();

    // Generate random vectors in parallel
    vector<thread> threads;
    for (int i = 0; i < num_partitions; i++) {
        int start_idx = i * partition_size;
        int end_idx = (i == num_partitions - 1) ? size : start_idx + partition_size;
        threads.push_back(thread(randomVector, v1, start_idx, end_idx));
        threads.push_back(thread(randomVector, v2, start_idx, end_idx));
    }

    // Wait for all threads to finish
    for (auto &t : threads) {
        t.join();
    }

    threads.clear(); // Clear the threads for reuse

    // Perform vector addition in parallel
    for (int i = 0; i < num_partitions; i++) {
        int start_idx = i * partition_size;
        int end_idx = (i == num_partitions - 1) ? size : start_idx + partition_size;
        threads.push_back(thread(vectorAddition, v1, v2, v3, start_idx, end_idx));
    }

    // Wait for all threads to finish
    for (auto &t : threads) {
        t.join();
    }

    // Stop timing the execution
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Output the time taken by the function
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    // Free allocated memory
    free(v1);
    free(v2);
    free(v3);

    return 0;
}
