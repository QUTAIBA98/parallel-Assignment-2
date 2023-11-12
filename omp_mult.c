#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>

#define DEBUG 0

int main(int argc, char* argv[])
{
    // Catch command line errors
    if (argc != 9)
    {
        printf("Usage: ./omp_mult  mat_1.csv n_row_1 n_col_1   mat_2.csv n_row_2 n_col_2  num_threads  results_matrix.csv\n");
        return EXIT_FAILURE;
    }
    printf("NOTE: For correct results, entered values of n_Rows and n_Cols of\n");
    printf("matrices must match the dimensions of matrices in .csv input files\n");

    // Get input files from the command line and open for reading
    FILE* inputMatrix1 = fopen(argv[1], "r");
    FILE* inputMatrix2 = fopen(argv[4], "r");

    // Get dimensions of 1st matrix from command line
    int n_row1 = strtol(argv[2], NULL, 10);
    int n_col1 = strtol(argv[3], NULL, 10);

    // Get dimensions of 2nd matrix from command line
    int n_row2 = strtol(argv[5], NULL, 10);
    int n_col2 = strtol(argv[6], NULL, 10);

    // Get number of threads from command line
    int thread_count = strtol(argv[7], NULL, 10);

    // Get name of output file from command line and open for writing
    FILE* outputFile = fopen(argv[8], "w");

    // Initialize the two input matrices and the resultant matrix
    long int* matrix1 = (long int*)malloc((n_row1 * n_col1) * sizeof(long int));
    long int* matrix2 = (long int*)malloc((n_row2 * n_col2) * sizeof(long int));
    long int* result = (long int*)malloc((n_row1 * n_col2) * sizeof(long int));

    // Parse the input csv files and fill in the input matrices
    for (int i = 0; i < n_row1; i++) {
        for (int j = 0; j < n_col1; j++) {
            fscanf(inputMatrix1, "%ld,", &matrix1[i * n_col1 + j]);
        }
    }
    for (int i = 0; i < n_row2; i++) {
        for (int j = 0; j < n_col2; j++) {
            fscanf(inputMatrix2, "%ld,", &matrix2[i * n_col2 + j]);
        }
    }

    // Record the start time
    double start = omp_get_wtime();

    // Perform matrix multiplication in parallel
#   pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < n_row1; i++) {
        for (int j = 0; j < n_col2; j++) {
            result[i * n_col2 + j] = 0;
            for (int k = 0; k < n_row2; k++) {
                result[i * n_col2 + j] += matrix1[i* n_row2 + k] * matrix2[k * n_col2 + j];
            }
        }
    }

    // Record the finish time        
    double end = omp_get_wtime();

    // Measure execution time
    double execution_time = end - start;

    // Write the resultant matrix to the results file
    for (int i = 0; i < n_row1; i++) {
        for (int j = 0; j < n_col2; j++) {
            fprintf(outputFile, "%ld,", result[i * n_col2 + j]);
        }
        fprintf(outputFile, "\n");
    }

    // Print the execution time
    printf("\nDone! Execution time:  %f  seconds\n\n", execution_time);

    // Close the files
    fclose(inputMatrix1);
    fclose(inputMatrix2);
    fclose(outputFile);

    // Free memory
    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}

