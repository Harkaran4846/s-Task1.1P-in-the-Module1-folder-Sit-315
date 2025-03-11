#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 3 // Change as needed
#define MAX_THREADS N

int A[N][N], B[N][N], C[N][N];

void *multiply_row(void *arg) {
    int i = *(int *)arg;
    for (int j = 0; j < N; j++) {
        C[i][j] = 0;
        for (int k = 0; k < N; k++) {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
    pthread_exit(0);
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
    pthread_t threads[MAX_THREADS];
    int row_indices[MAX_THREADS];

    generate_matrix(A);
    generate_matrix(B);

    clock_t start = clock();

    for (int i = 0; i < MAX_THREADS; i++) {
        row_indices[i] = i;
        pthread_create(&threads[i], NULL, multiply_row, &row_indices[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    write_matrix_to_file(C, "output_parallel_pthreads.txt");

    printf("Time taken: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}
