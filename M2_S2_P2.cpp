#include <chrono>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <thread>
#include <vector>

using namespace std::chrono;
using namespace std;

// Function to fill a vector with random values
void randomVector(int vector[], int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 100; // Assign random values between 0 and 99
  }
}

// Thread function to perform vector addition on a chunk
void vector_add(int start, int end, int *v1, int *v2, int *v3) {
  for (int i = start; i < end; i++) {
    v3[i] = v1[i] + v2[i]; // Add elements in the assigned chunk
  }
}

int main() {
  unsigned long size = 100000000; // Size of the vectors
  srand(time(0)); // Seed the random number generator

  int *v1, *v2, *v3; // Declare pointers for vectors v1, v2, and v3

  // Allocate memory for the vectors
  v1 = (int *)malloc(size * sizeof(int));
  v2 = (int *)malloc(size * sizeof(int));
  v3 = (int *)malloc(size * sizeof(int));

  // Fill vectors v1 and v2 with random values
  randomVector(v1, size);
  randomVector(v2, size);

  // Start timing the execution
  auto start = high_resolution_clock::now();

  // Number of threads to use
  const int num_threads = 4;
  vector<thread> threads;
  int chunk_size = size / num_threads;

  // Create threads to perform vector addition in parallel
  for (int i = 0; i < num_threads; i++) {
    int start = i * chunk_size;
    int end = (i == num_threads - 1) ? size : start + chunk_size;
    threads.push_back(thread(vector_add, start, end, v1, v2, v3));
  }

  // Join threads (wait for all threads to finish)
  for (auto &th : threads) {
    th.join();
  }

  // Stop timing the execution
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  // Output the execution time
  cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

  // Free allocated memory
  free(v1);
  free(v2);
  free(v3);

  return 0;
}
