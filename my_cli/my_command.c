#include <string.h>
#include "my_command.h"

parsed_command_t parse_string(char *str) {
    int i, j=0;
    parsed_command_t command;
    command.argc = 0;
    
    for (i=0; i<strlen(str); i++) {
        if (str[i] == ' ' || str[i] == '\0') {
            command.args[command.argc][j] = '\0';
            command.argc++;
            j = 0;
        } else {
            command.args[command.argc][j] = str[i];
            j++;
        }
    }

    return command;
}