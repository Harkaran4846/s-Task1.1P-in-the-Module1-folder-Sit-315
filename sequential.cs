#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 3 // Change this to test different sizes

void multiply_matrices(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void generate_matrix(int M[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M[i][j] = rand() % 10;
}

void write_matrix_to_file(int M[N][N], const char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%d ", M[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    srand(time(NULL));
    int A[N][N], B[N][N], C[N][N];

    generate_matrix(A);
    generate_matrix(B);

    clock_t start = clock();
    multiply_matrices(A, B, C);
    clock_t end = clock();

    write_matrix_to_file(C, "output_sequential.txt");

    printf("Time taken: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
