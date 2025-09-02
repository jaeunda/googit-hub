#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commands.h"

void execute_push(const char *msg){
    printf("[PUSH] %s\n", msg);
    return;
}