#include "cli_arguments.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_LOOPS 3

cli_ret_val get_cli_arguments(const int argc, char ***const argv, cli_arguments *const cli_args) {
    int flag;

    if (argv && cli_args) {
        cli_args->input_file_name_1 = NULL;
        cli_args->input_file_name_2 = NULL;
        cli_args->output_file_name = NULL;
        cli_args->use_parallel = 0u;
        cli_args->which_loop = 0;

        while(-1 != (flag = getopt(argc, *argv, "1:2:o:pl:"))) {
            switch(flag) {
                case '1':
                    cli_args->input_file_name_1 = optarg;
                    break;
                case '2':
                    cli_args->input_file_name_2 = optarg;
                    break;
                case 'o':
                    cli_args->output_file_name = optarg;
                    break;
                case 'p':
                    cli_args->use_parallel = 1u;
                    break;
                case 'l':
                    int loop = atoi(optarg);
                    cli_args->which_loop = (0 <= loop && NUMBER_OF_LOOPS > loop) ? loop : 0;
                    break;
                case '?':
                    if ('1' == optopt || '2' == optopt || 'o' == optopt || 'l' == optopt) {
                        fprintf(stderr, "Option '-%c' requires an argument.\n", optopt);
                        return MISSING_PARAMETER;
                    } else {
                        fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                        return UNKNOWN_OPTION;
                    }
                // flows through
                default:
                    return CLI_NOT_OK;
            }
        }
        return CLI_OK;
    }
    return CLI_NULL_PTR_PASSED;
}
