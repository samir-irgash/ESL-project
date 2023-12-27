#ifndef __MY_COMMAND
#define __MY_COMMAND

#define ARGS_MAX_NUMBER 4
#define ARGS_MAX_LENGTH 5

typedef struct {
    char args[ARGS_MAX_NUMBER][ARGS_MAX_LENGTH];
    int argc;
} parsed_command_t;

parsed_command_t parse_string(
    char *str
);

#endif