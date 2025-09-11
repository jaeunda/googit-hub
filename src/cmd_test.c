#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "commands.h"
#include "utils.h"

void execute_test(void){
    char project_num[8];
    char student_id[16];
    char class_num[8];
    char version_str[8];

    if (read_config("project_num", project_num, sizeof(project_num)) < 0 ||
        read_config("student_id", student_id, sizeof(student_id)) < 0 ||
        read_config("class_num", class_num, sizeof(class_num)) < 0 ||
        read_config("version", version_str, sizeof(version_str)) < 0){
        fprintf(stderr, "Error: Could not read data from config file.\n");
        exit(EXIT_FAILURE);
    }
    int version = atoi(version_str);
    char test_filename[256];

    // 0. init: filename, current path
    if (!version){
        fprintf(stdout, "There is no file to test.\n");
        exit(EXIT_SUCCESS);
    } else if (version == 1){
        snprintf(test_filename, sizeof(test_filename), "#%s_%s_%s.zip",
            project_num, student_id, class_num);
    } else {
        snprintf(test_filename, sizeof(test_filename), "#%s_%s_%s_V%d.zip",
            project_num, student_id, class_num, version);
    }

    char current_path[4096];
    if (!getcwd(current_path, sizeof(current_path))){
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    // 1. make temporary directory
    char run_path[256];
    snprintf(run_path, sizeof(run_path), "%s/.googit/run-xv6", current_path);
    char cmd_rmdir[512];
    snprintf(cmd_rmdir, sizeof(cmd_rmdir), "rm -rf %s", run_path);
    
    struct stat st = {0};
    if (!stat(run_path, &st) && S_ISDIR(st.st_mode)){
        run_command(cmd_rmdir);
    }

    if (mkdir(run_path, 0755) < 0){
        perror(".googit/run-xv6");
        exit(EXIT_FAILURE);
    }
    if (run_command("cp -a .googit/original/* .googit/run-xv6")){
        fprintf(stderr, "Error: Failed to create a temporary directory.\n");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 2. unzip && overwrite
    char cmd_unzip[1024];
    snprintf(cmd_unzip, sizeof(cmd_unzip), "unzip -o \"%s\" -d %s", test_filename, run_path);

    if (run_command(cmd_unzip)){
        fprintf(stderr, "Error: Failed to unzip the file.\n");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 3. change directory
    if (chdir(run_path) < 0){
        perror("test-chdir");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 4. make && make qemu
    if (run_command("make && make qemu-nox")){
        fprintf(stderr, "Error: Build Failed.\n");
    }

    // 5. make clean
    if (run_command("make clean")){
        perror("make clean");
    }

    // 6. change directory
    if (chdir(current_path) < 0){
        perror("test-chdir");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 7. remove test directory
    run_command(cmd_rmdir);

}