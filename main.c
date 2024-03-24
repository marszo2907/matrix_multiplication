#include "cli_arguments.h"
#include "matrix_multiplication.h"
#include "return_values.h"

#include <stdio.h>

#define FILE_READ_NUM_THREADS 2

int main(int argc, char **argv) {
    cli_args cli_args;
    ret_val cli_ret_val;
    matrix matrix1;
    matrix matrix2;
    matrix result;
    int main_ret_val;

    if (OK == (cli_ret_val = get_cli_args(argc, &argv, &cli_args))) {
        init_matrix(&matrix1);
        init_matrix(&matrix2);
        init_matrix(&result);

        ret_val read_file_1_ret_val;
        ret_val read_file_2_ret_val;
        #pragma omp parallel sections shared(read_file_1_ret_val, read_file_2_ret_val) num_threads(FILE_READ_NUM_THREADS)
        {
            #pragma omp section
            {
                read_file_1_ret_val = read_matrix(&matrix1, cli_args.input_file_name_1);
            }
            #pragma omp section
            {
                read_file_2_ret_val = read_matrix(&matrix2, cli_args.input_file_name_2);
            }
        }

        if (OK == read_file_1_ret_val && OK == read_file_2_ret_val) {
            ret_val matrix_multiplicaiton_ret_val = multiply_matrices(&matrix1, &matrix2, &result, &cli_args);
            if (OK == matrix_multiplicaiton_ret_val) {
                ret_val save_result_ret_val = save_matrix(&result, cli_args.output_file_name);
                if (OK != save_result_ret_val) {
                    fprintf(stderr, "There was an error in saving matrix to %s, return value = %d.\n", cli_args.output_file_name, save_result_ret_val);
                }
                main_ret_val = (int) save_result_ret_val;
            } else {
                fprintf(stderr, "There was an error in matrix multiplication, return value = %d.\n", matrix_multiplicaiton_ret_val);
                main_ret_val = (int) matrix_multiplicaiton_ret_val;
            }
        } else if (OK == read_file_1_ret_val) {
            fprintf(stderr, "There was an error in reading matrix from %s, return value = %d.\n",
                cli_args.input_file_name_2, read_file_2_ret_val);
            main_ret_val = (int) read_file_2_ret_val;
        } else if (OK == read_file_2_ret_val) {
            fprintf(stderr, "There was an error in reading matrix from %s, return value = %d.\n",
                cli_args.input_file_name_1, read_file_1_ret_val);
            main_ret_val = (int) read_file_1_ret_val;
        } else {
            fprintf(stderr, "There was an error in reading matrices from %s and %s, return values = %d, %d.\n",
                cli_args.input_file_name_1, cli_args.input_file_name_2, read_file_1_ret_val, read_file_1_ret_val);
            main_ret_val = (int) read_file_1_ret_val;
        }

        deinit_matrix(&matrix1);
        deinit_matrix(&matrix2);
        deinit_matrix(&result);
    } else {
        fprintf(stderr, "There was an error with parsing command line arguments, return value: %d\n", cli_ret_val);
        main_ret_val = (int) cli_ret_val;
    }

    return main_ret_val;
}
