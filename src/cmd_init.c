#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commands.h"

void execute_init(const char *project_num, const char *student_id, const char *class_num){
    printf("[INIT] %s %s %s\n", project_num, student_id, class_num);
    return;
}