#ifndef MATRIX_MULTIPLICATION_H
#define MATRIX_MULTIPLICATION_H

typedef struct Matrix {
    int rows;
    int columns;
    int **content;
} Matrix;

void deinitMatrix(Matrix *const matrixPtr);
void initMatrix(Matrix *const matrixPtr);
int multiplyMatrices(const Matrix *const m1, const Matrix *const m2, Matrix *const result);
void printMatrix(const Matrix *const matrixPtr);
int readMatrix(Matrix *const matrixPtr, const char *const fileName);
int saveMatrix(const Matrix *const matrix, const char *const fileName);

#endif // MATRIX_MULTIPLICATION_H
