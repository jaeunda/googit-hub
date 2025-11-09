#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "commands.h"
#include "utils.h"

void execute_test(const char *filename, int is_clean){
    char project_num[8];
    char student_id[16];
    char class_num[8];
    char version_str[8];
    int version;

    if (filename == NULL && 
        (read_config("project_num", project_num, sizeof(project_num)) < 0 ||
        read_config("student_id", student_id, sizeof(student_id)) < 0 ||
        read_config("class_num", class_num, sizeof(class_num)) < 0 ||
        read_config("version", version_str, sizeof(version_str)) < 0)){
        fprintf(stderr, "Error: Could not read data from config file.\n");
        exit(EXIT_FAILURE);
    } else {
        version = atoi(version_str);
    }
    char test_filename[256];

    // 0. init: filename, current path
    if (filename){
        strncpy(test_filename, filename, sizeof(test_filename));
    } else if (!version){
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
    snprintf(run_path, sizeof(run_path), "%s/run-xv6", current_path);
    char cmd_rmdir[512];
    snprintf(cmd_rmdir, sizeof(cmd_rmdir), "rm -rf %s", run_path);
    
    int is_reboot = 0;
    struct stat st = {0};
    if (!stat(run_path, &st) && S_ISDIR(st.st_mode)){
        is_reboot = 1;
    }

    if (!is_reboot && mkdir(run_path, 0755) < 0){
        perror("run-xv6");
        exit(EXIT_FAILURE);
    }
    if (!is_reboot && run_command("cd run-xv6 && git clone https://github.com/mit-pdos/xv6-public.git && cd ..")){
        fprintf(stderr, "Error: Failed to create a temporary directory.\n");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 2. unzip && overwrite
    char cmd_unzip[1024];
    snprintf(cmd_unzip, sizeof(cmd_unzip), "unzip -o \"%s\" -d unzip_dir", test_filename);

    if (!is_reboot && run_command(cmd_unzip)){
        fprintf(stderr, "Error: Failed to unzip the file.\n");
        run_command(cmd_rmdir);
        run_command("rm -rf unzip_dir");
        exit(EXIT_FAILURE);
    }

    if (!is_reboot && run_command("cp -a unzip_dir/소스코드/* run-xv6/xv6-public")){
        if (run_command("cp -a unzip_dir/* run-xv6/xv6-public")){
            fprintf(stderr, "Error: Failed to copy source file.\n");
            run_command(cmd_rmdir);
            run_command("rm -rf unzip_dir");
            exit(EXIT_FAILURE);
        }
    }

    if (!is_reboot && run_command("rm -rf unzip_dir")){
        fprintf(stderr, "Error: Failed to remove unzip directory.\n");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 3. change directory
    if (chdir("run-xv6/xv6-public") < 0){
        perror("test-chdir");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 4. make && make qemu
    if (run_command("make && make qemu-nox")){
        fprintf(stderr, "Error: Build Failed.\n");
    }

    // 5. make clean
    if (is_clean && run_command("make clean")){
        perror("make clean");
    }

    // 6. change directory
    if (is_clean && chdir(current_path) < 0){
        perror("test-chdir");
        run_command(cmd_rmdir);
        exit(EXIT_FAILURE);
    }

    // 7. remove test directory
    if (is_clean)
        run_command(cmd_rmdir);

}