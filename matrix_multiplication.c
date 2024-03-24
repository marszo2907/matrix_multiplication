#include "matrix_multiplication.h"

#include <ctype.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHAR_TO_INT_OFFSET '0'
#define DEF_DIMENTIONS_VALUE -1
#define DEF_LINE_BUFFER_SIZE 1024
#define NUMBER_BUFFER_SIZE 11

void deinit_matrix(matrix *const matrix) {
    if (matrix && matrix->content) {
        for (int row = 0; matrix->rows > row; ++row) {
            free(matrix->content[row]);
        }
        free(matrix->content);
    }
}

void init_matrix(matrix *const matrix) {
    if (matrix) {
        matrix->rows = DEF_DIMENTIONS_VALUE;
        matrix->columns = DEF_DIMENTIONS_VALUE;
        matrix->content = NULL;
    }
}

ret_val multiply_matrices(const matrix *const m1, const matrix *const m2, matrix *const result, const cli_args *const cli_args) {
    if (m1 && m1->content && m2 && m2->content && result && cli_args) {
        if (m1->columns == m2->rows) {
            result->rows = m1->rows;
            result->columns = m2->columns;
            result->content = (int **) malloc(result->rows * sizeof(int *));
            for (int row = 0; result->rows > row; ++row) {
                result->content[row] = (int *) malloc(result->columns * sizeof(int));
                memset(result->content[row], 0, result->columns * sizeof(int));
            }

            double begin = omp_get_wtime();
            #pragma omp parallel for shared(m1, m2, result) if(cli_args->use_parallel && !cli_args->parallelize_inner) num_threads(cli_args->num_threads)
            for (int i = 0; result->rows > i; ++i) {
                #pragma omp parallel for shared(m1, m2, result) if(cli_args->use_parallel && cli_args->parallelize_inner) num_threads(cli_args->num_threads)
                for (int j = 0; result->columns > j; ++j) {
                    for (int k = 0; m1->columns > k; ++k) {
                        result->content[i][j] += m1->content[i][k] * m2->content[k][j];
                    }
                }
            }
            double end = omp_get_wtime();
            if (cli_args->print_logs) {
                printf("use_parallel | parallelize_inner | num_threads | size | time [s]\n");
                printf("%12d | %17d | %11d | %4d | %8lf\n",
                    cli_args->use_parallel, cli_args->parallelize_inner, cli_args->num_threads, m1->columns, end - begin);
            }

            return OK;
        }
        return INCOMPATIBLE_MATRICES_SIZES;
    }
    return NULL_PTR_PASSED;
}

void print_matrix(const matrix *const matrix) {
    if (matrix) {
        for (int i = 0; matrix->rows > i; ++i) {
            for (int j = 0; matrix->columns > j; ++j) {
                printf("[%3d] ", matrix->content[i][j]);
            }
            printf("\n");
        }
    }
}

ret_val read_matrix(matrix *const matrix, const char *const file_name) {
    int bytes_read;
    int column = 0;
    FILE *file;
    char *line_buffer;
    long unsigned int line_buffer_size = DEF_LINE_BUFFER_SIZE;
    char number_buffer[NUMBER_BUFFER_SIZE];
    int number_buffer_idx = 0;
    int row = 0;

    if (matrix && (file = fopen(file_name, "r"))) {
        line_buffer = malloc(line_buffer_size * sizeof(char));
        while (EOF != (bytes_read = getline(&line_buffer, &line_buffer_size, file))) {
            for (int i = 0; bytes_read > i; ++i) {
                if ((NUMBER_BUFFER_SIZE - 1 > number_buffer_idx)
                    && ('-' == line_buffer[i] || isnumber(line_buffer[i]))) {
                    number_buffer[number_buffer_idx++] = line_buffer[i];
                } else if (0 != number_buffer_idx) {
                    number_buffer[number_buffer_idx] = '\0';
                    number_buffer_idx = 0;
                    int number = atoi(number_buffer);

                    if (DEF_DIMENTIONS_VALUE == matrix->rows) {
                        matrix->rows = number;
                    } else if (DEF_DIMENTIONS_VALUE == matrix->columns) {
                        matrix->columns = number;
                        matrix->content = (int **) malloc(matrix->rows * sizeof(int *));
                        for (row = 0; matrix->rows > row; ++row) {
                            matrix->content[row] = (int *) malloc(matrix->columns * sizeof(int));
                        }
                        row = 0;
                    } else {
                        if (matrix->columns <= column) {
                            row++;
                            column = 0;
                        }
                        if (matrix->rows > row) {
                            matrix->content[row][column++] = number;
                        }
                    }
                }
            }
        }
        fclose(file);
        free(line_buffer);

        if (matrix->rows - 1 == row && matrix->columns == column) {
            return OK;
        } else {
            return INVALID_FILE_CONTENT;
        }
    }
    if (matrix) {
        return CANT_OPEN_FILE;
    }
    return NULL_PTR_PASSED;
}

ret_val save_matrix(const matrix *const matrix, const char *const file_name) {
    FILE *file;

    if (matrix && matrix->content && (file = fopen(file_name, "w"))) {
        fprintf(file, "%d %d\n", matrix->rows, matrix->columns);
        for (int i = 0; matrix->rows > i; ++i) {
            for (int j = 0; matrix->columns > j; ++j) {
                fprintf(file, "%d ", matrix->content[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);

        return OK;
    }
    if (matrix && matrix->content) {
        return CANT_OPEN_FILE;
    }
    return NULL_PTR_PASSED;
}
