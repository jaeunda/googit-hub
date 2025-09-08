#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "commands.h"
#include "utils.h"

void execute_init(const char *project_num, const char *student_id, const char *class_num){
    // 1. .googit/ 
    struct stat st = {0};
    if (!stat(".googit", &st) && S_ISDIR(st.st_mode)){
        fprintf(stdout, "Notice: A googit project already exists in this directory\n");
        return;
    }
    printf("Initializing new googit project...\n");
    if (mkdir(".googit", 0755) < 0){
        perror(".googit directory");
        exit(EXIT_FAILURE);
    }
    // 2. .googit/output_dir
    if (mkdir(".googit/output_dir", 0755) < 0){
        perror(".googit/output_dir");
        exit(EXIT_FAILURE);
    }
    // 3. .googit/googit_config init
    if (write_config("project_num", project_num) != 0 ||
        write_config("student_id", student_id) != 0 ||
        write_config("class_num", class_num) != 0 ||
        write_config("version", "0") != 0) {
            fprintf(stderr, "Error: Failed to write config file\n");
            rmdir(".googit/output_dir");
            rmdir(".googit");
            exit(EXIT_FAILURE);
        }
    
    // 3-2. Set up google drive configuration
    setup_gdrive();

    // 4. git clone <xv6-public>
    struct stat st2 = {0};
    if (!stat("xv6-public", &st2) && S_ISDIR(st.st_mode)){
        run_command("rm -rf xv6-public");
    }
    if (run_command("git clone https://github.com/mit-pdos/xv6-public.git") != 0){
        fprintf(stderr, "Error: Failed to clone xv6-public\n");
        remove(".googit/googit_config");
        rmdir(".googit/output_dir");
        rmdir(".googit");
        exit(EXIT_FAILURE);
    }
    // 5. original/ : baseline
    if (run_command("cp -a xv6-public/ .googit/original/") != 0){
        fprintf(stderr, "Error: Failed to create a prinstine copy\n");
        run_command("rm -rf xv6-public .googit");
        exit(EXIT_FAILURE);
    }

    printf("\nGoogit project initialized.\n");
    return;
}