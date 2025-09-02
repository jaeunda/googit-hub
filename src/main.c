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
        if (argc != 5){
            fprintf(stderr, "Error: Incorrect arguments for init.\n");
            fprintf(stderr, "Usage: googit init <project_num> <student_id> <class_num>");
            exit(1);
        }
        execute_init(argv[2], argv[3], argv[4]);
    } else if (!strcmp(command, "push")){
        if (argc == 4 && !strcmp(argv[2], "-m")){
            execute_push(argv[3]);
        } else if (argc == 2){
            execute_push(NULL);
        } else {
            fprintf(stderr, "Error: Incorrect arguments for push.\n");
            fprintf(stderr, "Usage: googit push [-m \"message\"]\n");
            exit(1);
        }
    } else if (!strcmp(command, "run")){
        execute_run();
    } else if (!strcmp(command, "zip")){
        if (argc == 3 && !strcmp(argv[2], "--revert")){
            execute_zip(1);
        } else if (argc == 2){
            execute_zip(0);
        } else {
            fprintf(stderr, "Error: Incorrect arguments for zip.\n");
            fprintf(stderr, "Usage: googit zip [--revert]\n");
            exit(1);
        }
    } else if (!strcmp(command, "test")){
        execute_test();
    } else if (!strcmp(command, "restore")){
        execute_restore();
    } else if (!strcmp(command, "--help") || !strcmp(command, "help")){
        print_usage();
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