#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h> // Corrected from ctime to time.h

#define N 3 // Change as needed

int A[N][N], B[N][N], C[N][N];

void generate_matrix(int M[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M[i][j] = rand() % 10;
}

void write_matrix_to_file(int M[N][N], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%d ", M[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    srand(time(NULL)); // Initialize random seed

    generate_matrix(A);
    generate_matrix(B);

    double start = omp_get_wtime(); // OpenMP timing function

    #pragma omp parallel for collapse(2) // Optimized parallelization
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double end = omp_get_wtime();

    write_matrix_to_file(C, "output_parallel_openmp.txt");

    printf("Time taken: %lf seconds\n", end - start);
    return 0;
}