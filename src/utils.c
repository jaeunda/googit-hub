#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"

#define CONFIG_PATH ".googit/.googit_config"
#define LINE_MAX 32
#define LEN_MAX 128

int run_command(const char *command){
    printf("Executing: %s\n", command);
    int status;
    if ((status = system(command)) != 0){
        fprintf(stderr, "Error: Command failed with status %d\n", status);
    }
    return status;
}