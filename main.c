#include "cli_arguments.h"
#include "matrix_multiplication.h"

#include <stdio.h>

int main(int argc, char **argv) {
    cli_arguments cli_args;
    cli_ret_val cli_ret_val;
    Matrix matrix1;
    Matrix matrix2;
    Matrix result;
    int ret_val;

    if (MATRIX_OK == (cli_ret_val = get_cli_arguments(argc, &argv, &cli_args))) {
        initMatrix(&matrix1);
        initMatrix(&matrix2);
        initMatrix(&result);

        matrix_ret_val open_file_1_ret_val = readMatrix(&matrix1, cli_args.input_file_name_1);
        matrix_ret_val open_file_2_ret_val = readMatrix(&matrix2, cli_args.input_file_name_2);
        if (MATRIX_OK == open_file_1_ret_val && MATRIX_OK == open_file_2_ret_val) {
            matrix_ret_val matrix_multiplicaiton_ret_val = multiplyMatrices(&matrix1, &matrix2, &result);
            if (MATRIX_OK == matrix_multiplicaiton_ret_val) {
                matrix_ret_val save_result_ret_val = saveMatrix(&result, cli_args.output_file_name);
                if (MATRIX_OK != save_result_ret_val) {
                    fprintf(stderr, "There was an error in saving matrix to %s, return value = %d.\n", cli_args.output_file_name, save_result_ret_val);
                }
                ret_val = (int) save_result_ret_val;
            } else {
                fprintf(stderr, "There was an error in matrix multiplication, return value = %d.\n", matrix_multiplicaiton_ret_val);
                ret_val = (int) matrix_multiplicaiton_ret_val;
            }
        } else if (MATRIX_OK == open_file_1_ret_val) {
            fprintf(stderr, "There was an error in reading matrix from %s, return value = %d.\n",
                cli_args.input_file_name_2, open_file_2_ret_val);
            ret_val = (int) open_file_2_ret_val;
        } else if (MATRIX_OK == open_file_2_ret_val) {
            fprintf(stderr, "There was an error in reading matrix from %s, return value = %d.\n",
                cli_args.input_file_name_1, open_file_1_ret_val);
            ret_val = (int) open_file_1_ret_val;
        } else {
            fprintf(stderr, "There was an error in reading matrices from %s and %s, return values = %d, %d.\n",
                cli_args.input_file_name_1, cli_args.input_file_name_2, open_file_1_ret_val, open_file_2_ret_val);
            ret_val = (int) open_file_1_ret_val;
        }

        deinitMatrix(&matrix1);
        deinitMatrix(&matrix2);
        deinitMatrix(&result);
    } else {
        fprintf(stderr, "There was an error with parsing command line arguments, return value: %d\n", cli_ret_val);
        ret_val = (int) cli_ret_val;
    }

    return ret_val;
}
