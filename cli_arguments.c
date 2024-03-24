#include "cli_arguments.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEF_NUM_THREADS 1
#define FALSE 0u
#define NUM_LOOPS 2u
#define TRUE 1u

ret_val get_cli_args(const int argc, char ***const argv, cli_args *const cli_args) {
    int flag;

    if (argv && cli_args) {
        cli_args->input_file_name_1 = NULL;
        cli_args->input_file_name_2 = NULL;
        cli_args->output_file_name = NULL;
        cli_args->use_parallel = FALSE;
        cli_args->parallelize_inner = FALSE;
        cli_args->print_logs = FALSE;
        cli_args->num_threads = DEF_NUM_THREADS;

        while(-1 != (flag = getopt(argc, *argv, "1:2:o:piln:"))) {
            switch(flag) {
                case '1':
                    if (NULL == cli_args->input_file_name_1) {
                        cli_args->input_file_name_1 = optarg;
                    }
                    break;
                case '2':
                    if (NULL == cli_args->input_file_name_2) {
                        cli_args->input_file_name_2 = optarg;
                    }
                    break;
                case 'o':
                    cli_args->output_file_name = optarg;
                    break;
                case 'p':
                    cli_args->use_parallel = TRUE;
                    break;
                case 'i':
                    cli_args->parallelize_inner = TRUE;
                    break;
                case 'l':
                    cli_args->print_logs = TRUE;
                    break;
                case 'n':
                    int num_threads = atoi(optarg);
                    cli_args->num_threads = 0 < num_threads ? num_threads : DEF_NUM_THREADS;
                    break;
                case '?':
                    if ('1' == optopt || '2' == optopt || 'o' == optopt || 'n' == optopt) {
                        fprintf(stderr, "Option '-%c' requires an argument.\n", optopt);
                        return MISSING_CLI_PARAMETER;
                    } else {
                        fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                        return UNKNOWN_CLI_OPTION;
                    }
                // flows through
                default:
                    return NOT_OK;
            }
        }
        return OK;
    }
    return NULL_PTR_PASSED;
}
