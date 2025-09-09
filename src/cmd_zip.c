#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commands.h"
#include "utils.h"

void execute_zip(int is_revert){
    // 0. --revert
    if (is_revert){
        char version_str[8];
        if (read_config("version", version_str, sizeof(version_str)) < 0){
            fprintf(stderr, "Error: Could not read config file.\n");
            exit(EXIT_FAILURE);
        }

        int version = atoi(version_str);
        if (version > 0){
            version--;
            snprintf(version_str, sizeof(version_str), "%d", version);
            if (write_config("version", version_str) < 0){
                fprintf(stderr, "Error: Could not update version.\n");
                exit(EXIT_FAILURE);
            }
            printf("Version reverted to %d.\n", version);
        } else {
            printf("Version is already 0.\n");
        }
    }

    // 1. make filename

    // 2. zip

    // 3. version++
}