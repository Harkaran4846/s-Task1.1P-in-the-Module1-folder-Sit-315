#include <chrono>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <thread> // Include the thread library for parallelization
#include <vector> // Include vector for easier partitioning

using namespace std::chrono;
using namespace std;

// Function to fill a vector with random values
void randomVector(int vector[], int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 100; // Assign random values between 0 and 99
  }
}

int main() {
  unsigned long size = 100000000; // Size of the vectors
  srand(time(0)); // Seed the random number generator

  int *v1, *v2, *v3; // Declare pointers for vectors v1, v2, and v3

  // Start timing the execution
  auto start = high_resolution_clock::now();

  // Allocate memory for the vectors
  v1 = (int *)malloc(size * sizeof(int));
  v2 = (int *)malloc(size * sizeof(int));
  v3 = (int *)malloc(size * sizeof(int));

  // Fill vectors v1 and v2 with random values
  randomVector(v1, size);
  randomVector(v2, size);

  // Perform vector addition sequentially
  for (int i = 0; i < size; i++) {
    v3[i] = v1[i] + v2[i]; // Add corresponding elements of v1 and v2, store in v3
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
