#include "matrix_multiplication.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_BUFFER_SIZE 11
#define CHAR_TO_INT_OFFSET 48
#define DEFAULT_DIMENTIONS_VALUE -1

void initMatrix(Matrix *const matrixPtr) {
    if (matrixPtr) {
        matrixPtr->rows = DEFAULT_DIMENTIONS_VALUE;
        matrixPtr->columns = DEFAULT_DIMENTIONS_VALUE;
        matrixPtr->content = NULL;
    }
}

void deinitMatrix(Matrix *const matrixPtr) {
    if (matrixPtr && matrixPtr->content) {
        for (int row = 0; matrixPtr->rows > row; ++row) {
            free(matrixPtr->content[row]);
        }
        free(matrixPtr->content);
    }
}

void printMatrix(const Matrix *const matrixPtr) {
    if (matrixPtr) {
        for (int i = 0; matrixPtr->rows > i; ++i) {
            for (int j = 0; matrixPtr->columns > j; ++j) {
                printf("[%3d] ", matrixPtr->content[i][j]);
            }
            printf("\n");
        }
    }
}

int readMatrix(Matrix *const matrixPtr, const char *const fileName) {
    char byte;
    int row = 0;
    int column = 0;
    FILE *file = fopen(fileName, "r");

    if (file && matrixPtr) {
        while (EOF != (byte = fgetc(file))) {
            if (isnumber(byte)) {
                if (DEFAULT_DIMENTIONS_VALUE == matrixPtr->rows) {
                    matrixPtr->rows = byte - CHAR_TO_INT_OFFSET;
                } else if (DEFAULT_DIMENTIONS_VALUE == matrixPtr->columns) {
                    matrixPtr->columns = byte - CHAR_TO_INT_OFFSET;
                    matrixPtr->content = (int **) malloc(matrixPtr->rows * sizeof(int *));
                    for (row = 0; matrixPtr->rows > row; ++row) {
                        matrixPtr->content[row] = (int *) malloc(matrixPtr->columns * sizeof(int));
                    }
                    row = 0;
                } else {
                    if (matrixPtr->columns <= column) {
                        row++;
                        column = 0;
                    }
                    if (matrixPtr->rows > row) {
                        matrixPtr->content[row][column++] = byte - CHAR_TO_INT_OFFSET;
                    }
                }
            }
        }
        fclose(file);
        if (matrixPtr->rows - 1 == row && matrixPtr->columns == column) {
            return 0;
        } else {
            return 2;
        }
    }
    return 1;
}

int multiplyMatrices(const Matrix *const m1, const Matrix *const m2, Matrix *const result) {
    if (m1 && m1->content && m2 && m2->content && result) {
        if (m1->columns == m2->rows) {
            result->rows = m1->rows;
            result->columns = m2->columns;
            result->content = (int **) malloc(result->rows * sizeof(int *));
            for (int row = 0; result->rows > row; ++row) {
                result->content[row] = (int *) malloc(result->columns * sizeof(int));
                memset(result->content[row], 0, result->columns * sizeof(int));
            }

            for (int i = 0; result->rows > i; ++i) {
                for (int j = 0; result->columns > j; ++j) {
                    for (int k = 0; m1->columns > k; ++k) {
                        result->content[i][j] += m1->content[i][k] * m2->content[k][j];
                    }
                }
            }

            return 0;
        }
        return 2;
    }
    return 1;
}

int saveMatrix(const Matrix *const matrixPtr, const char *const fileName) {
    FILE *file = fopen(fileName, "w");

    if (file && matrixPtr && matrixPtr->content) {
        fprintf(file, "%d %d\n", matrixPtr->rows, matrixPtr->columns);
        for (int i = 0; matrixPtr->rows > i; ++i) {
            for (int j = 0; matrixPtr->columns > j; ++j) {
                fprintf(file, "%d ", matrixPtr->content[i][j]);
            }
            fprintf(file, "\n");
            }
            fclose(file);
            return 0;
        }

    return 1;
}
