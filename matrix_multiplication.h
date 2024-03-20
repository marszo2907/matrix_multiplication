#ifndef MATRIX_MULTIPLICATION_H
#define MATRIX_MULTIPLICATION_H

typedef struct Matrix {
    int rows;
    int columns;
    int **content;
} Matrix;

typedef enum matrix_ret_val {
    MATRIX_OK,
    MATRIX_NOT_OK,
    MATRIX_NULL_PTR_PASSED,
    INCOMPATIBLE_MATRICES_SIZE,
    NULL_PTR_PASSED_OR_CANT_OPEN_FILE,
    INVALID_FILE_CONTENT
} matrix_ret_val;

void deinitMatrix(Matrix *const matrixPtr);
void initMatrix(Matrix *const matrixPtr);
matrix_ret_val multiplyMatrices(const Matrix *const m1, const Matrix *const m2, Matrix *const result);
void printMatrix(const Matrix *const matrixPtr);
matrix_ret_val readMatrix(Matrix *const matrixPtr, const char *const fileName);
matrix_ret_val saveMatrix(const Matrix *const matrix, const char *const fileName);

#endif // MATRIX_MULTIPLICATION_H
