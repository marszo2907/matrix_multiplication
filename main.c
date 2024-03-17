#include "matrix_multiplication.h"

#include <stdio.h>

#define EXPECTED_ARGC 4

int main(int argc, char **argv) {
    Matrix matrix1;
    Matrix matrix2;
    Matrix result;

    initMatrix(&matrix1);
    initMatrix(&matrix2);
    initMatrix(&result);

    if (EXPECTED_ARGC == argc) {
        int retVal;
        if (0 == (retVal = readMatrix(&matrix1, argv[1]))) {
            if (0 == (retVal = readMatrix(&matrix2, argv[2]))) {
                if (0 == multiplyMatrices(&matrix1, &matrix2, &result)) {
                    printMatrix(&result);
                    if (0 != (retVal = saveMatrix(&result, argv[3]))) {
                        printf("There was an error in saving matrix to %s, return value = %d.\n", argv[3], retVal);
                    }
                } else {
                    printf("There was an error in matrix multiplication, return value = %d.\n", retVal);
                }
            } else {
                printf("There is an error in %s, return value = %d.\n", argv[2], retVal);
            }
        } else {
            printf("There is an error in %s, return value = %d.\n", argv[1], retVal);
        }
    } else {
        printf("Usage: %s input_file_1.txt input_file_2.txt output_file.txt\n", argv[0]);
    }

    deinitMatrix(&matrix1);
    deinitMatrix(&matrix2);
    deinitMatrix(&result);
}
