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
            fprintf(stderr, "Usage: googit init <project_num> <student_id> <class_num>\n");
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
            fprintf(stderr, "Usage: googit push [ -m \"message\" ]\n");
            exit(1);
        }
    } else if (!strcmp(command, "run")){
        if (argc == 3 && !strcmp(argv[2], "--no-clean")){
            execute_run(0);
        } else if (argc == 2){
            execute_run(1);
        } else {
            fprintf(stderr, "Error: Incorrect arguments for test.\n");
            fprintf(stderr, "Usage: googit run [ --no-clean ]\n");
            exit(1);
        }
    } else if (!strcmp(command, "zip")){
        if (argc == 3 && !strcmp(argv[2], "--revert")){
            execute_zip(1);
        } else if (argc == 2){
            execute_zip(0);
        } else {
            fprintf(stderr, "Error: Incorrect arguments for zip.\n");
            fprintf(stderr, "Usage: googit zip [ --revert ]\n");
            exit(1);
        }
    } else if (!strcmp(command, "test")){
        if (argc == 3 && !strcmp(argv[2], "--no-clean")){
            execute_test(NULL, 0);
        } else if (argc == 4 && !strcmp(argv[2], "-f")){
            execute_test(argv[3], 1);
        } else if (argc == 5 && !strcmp(argv[2], "-f") && !strcmp(argv[4], "--no-clean")) {
            execute_test(argv[3], 0);
        } else if (argc == 5 && !strcmp(argv[2], "--no-clean") && !strcmp(argv[3], "-f")){
            execute_test(argv[4], 0);
        } else if (argc == 2){
            execute_test(NULL, 1);
        } else {
            fprintf(stderr, "Error: Incorrect arguments for test.\n");
            fprintf(stderr, "Usage: googit test [ -f \"filename\" ] [ --no-clean ]\n");
            exit(EXIT_FAILURE);
        }
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
    printf("  init <proj_num> <id> <class_num>          Initialize a new googit project\n");
    printf("  push [ -m \"message\" ]                   Backup modified files to Google Drive\n");
    printf("  run  [ --no-clean ]                       Run the project in a test environment\n");
    printf("  zip [ --revert ]                          Create a submission zip file or revert version\n");
    printf("  test [ -f \"filename\" ] [ --no-clean ]   Test the latest submission zip file\n");
    printf("  restore                                   Restore files from a previous backup\n");
    printf("\n");
}