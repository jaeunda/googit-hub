#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"

void print_usage(void);

int main(int argc, char **argv){
    if (argc < 2){
        print_usage();
        exit(1);
    }
    char *command = argv[1];

    if (!strcmp(command, "init")){

    } else if (!strcmp(command, "push")){

    } else if (!strcmp(command, "run")){

    } else if (!strcmp(command, "zip")){

    } else if (!strcmp(command, "test")){

    } else if (!strcmp(command, "restore")){

    } else {
        fprintf(stderr, "Error: Unknown command %s\n", command);
        print_usage();
        exit(1);
    }
    exit(0);
}
void print_usage(void){
    printf("Usage: googit <command> [options]\n\n");
    printf("Available commands:\n");
    printf("  init <proj_num> <id> <class_num>  Initialize a new googit project\n");
    printf("  push [-m \"message\"]             Backup modified files to Google Drive\n");
    printf("  run                             Run the project in a test environment\n");
    printf("  zip [--revert]                  Create a submission zip file or revert version\n");
    printf("  test                            Test the latest submission zip file\n");
    printf("  restore                         Restore files from a previous backup\n");
    printf("\n");
}