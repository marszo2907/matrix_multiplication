#include "matrix_multiplication.h"

#include <ctype.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_TO_INT_OFFSET '0'
#define DEFAULT_DIMENTIONS_VALUE -1
#define DEFAULT_LINE_BUFFER_SIZE 1024
#define NUMBER_BUFFER_SIZE 11

void deinitMatrix(Matrix *const matrixPtr) {
    if (matrixPtr && matrixPtr->content) {
        for (int row = 0; matrixPtr->rows > row; ++row) {
            free(matrixPtr->content[row]);
        }
        free(matrixPtr->content);
    }
}

void initMatrix(Matrix *const matrixPtr) {
    if (matrixPtr) {
        matrixPtr->rows = DEFAULT_DIMENTIONS_VALUE;
        matrixPtr->columns = DEFAULT_DIMENTIONS_VALUE;
        matrixPtr->content = NULL;
    }
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
    int bytesRead;
    int column = 0;
    FILE *file;
    char *lineBuffer;
    long unsigned int lineBufferSize = DEFAULT_LINE_BUFFER_SIZE;
    char numberBuffer[NUMBER_BUFFER_SIZE];
    int numberBufferIndex = 0;
    int row = 0;

    if (matrixPtr && (file = fopen(fileName, "r"))) {
        lineBuffer = malloc(lineBufferSize * sizeof(char));
        while (EOF != (bytesRead = getline(&lineBuffer, &lineBufferSize, file))) {
            for (int i = 0; bytesRead > i; ++i) {
                if ((NUMBER_BUFFER_SIZE - 1 > numberBufferIndex)
                    && ('-' == lineBuffer[i] || isnumber(lineBuffer[i]))) {
                    numberBuffer[numberBufferIndex++] = lineBuffer[i];
                } else if (0 != numberBufferIndex) {
                    numberBuffer[numberBufferIndex] = '\0';
                    numberBufferIndex = 0;
                    int number = atoi(numberBuffer);

                    if (DEFAULT_DIMENTIONS_VALUE == matrixPtr->rows) {
                        matrixPtr->rows = number;
                    } else if (DEFAULT_DIMENTIONS_VALUE == matrixPtr->columns) {
                        matrixPtr->columns = number;
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
                            matrixPtr->content[row][column++] = number;
                        }
                    }
                }
            }
        }
        fclose(file);
        free(lineBuffer);

        if (matrixPtr->rows - 1 == row && matrixPtr->columns == column) {
            return 0;
        } else {
            printMatrix(matrixPtr);
            return 2;
        }
    }
    return 1;
}

int saveMatrix(const Matrix *const matrixPtr, const char *const fileName) {
    FILE *file = fopen(fileName, "w");

    if (matrixPtr && matrixPtr->content && (file = fopen(fileName, "w"))) {
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
