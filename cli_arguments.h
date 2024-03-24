#ifndef CLI_ARGUMENTS_H
#define CLI_ARGUMENTS_H

#include "return_values.h"

typedef struct {
    char *input_file_name_1;
    char *input_file_name_2;
    char *output_file_name;
    unsigned int use_parallel: 1;
    unsigned int parallelize_inner: 1;
    unsigned int print_logs: 1;
    int num_threads;
} cli_args;

ret_val get_cli_args(const int argc, char ***argv, cli_args *const cli_args);

#endif // CLI_ARGUMENTS_H
