#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "commands.h"
#include "utils.h"

void execute_run(void){
    // 0. init
    char current_path[4096];
    if (!getcwd(current_path, sizeof(current_path))){
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    char run_path[4096];
    snprintf(run_path, sizeof(run_path), "%s/.googit/run-xv6", current_path);
    char cmd_rmdir[4096];
    snprintf(cmd_rmdir, sizeof(cmd_rmdir), "rm -rf %s", run_path);
    
    // 1. make temporary directory
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

    // 2. overwrite
    if (run_command("cp -a .googit/output_dir/* .googit/run-xv6/")){
        fprintf(stderr, "Error: Failed to copy modified files.\n");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 3. change current directory
    if (chdir(run_path) < 0){
        perror("run-chdir");
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

    // 6. remove temporary directory
    if (chdir(current_path) < 0){
        perror("run-chdir");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }
    run_command(cmd_rmdir);
    return;
}