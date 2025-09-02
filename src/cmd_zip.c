#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commands.h"

void execute_zip(int is_revert){
    if (is_revert){
        printf("[ZIP] Version Down\n");
    } else {
        printf("[ZIP] Version UP\n");
    }
    return;
}