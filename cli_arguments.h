#ifndef CLI_ARGUMENTS_H
#define CLI_ARGUMENTS_H

typedef struct cli_arguments {
    char *input_file_name_1;
    char *input_file_name_2;
    char *output_file_name;
    unsigned int use_parallel: 1;
    int which_loop;
    int num_threads;
} cli_arguments;

typedef enum cli_ret_val {
    CLI_OK,
    CLI_NOT_OK,
    CLI_NULL_PTR_PASSED,
    MISSING_PARAMETER,
    UNKNOWN_OPTION
} cli_ret_val;

cli_ret_val get_cli_arguments(const int argc, char ***argv, cli_arguments *const cli_args);

#endif // CLI_ARGUMENTS_H
