#ifndef MATRIX_MULTIPLICATION_H
#define MATRIX_MULTIPLICATION_H

#include "cli_arguments.h"
#include "return_values.h"

typedef struct {
    int rows;
    int columns;
    int **content;
} matrix;

void deinit_matrix(matrix *const matrix);
void init_matrix(matrix *const matrix);
ret_val multiply_matrices(const matrix *const m1, const matrix *const m2, matrix *result, const cli_args *const cli_args);
void print_matrix(const matrix *const matrix);
ret_val read_matrix(matrix *const matrix, const char *const file_name);
ret_val save_matrix(const matrix *const matrix, const char *const file_name);

#endif // MATRIX_MULTIPLICATION_H
