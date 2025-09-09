#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commands.h"
#include "utils.h"

void execute_zip(int is_revert){
    // 0. --revert
    if (is_revert){
        char version_str[8];
        if (read_config("version", version_str, sizeof(version_str)) < 0){
            fprintf(stderr, "Error: Could not read config file.\n");
            exit(EXIT_FAILURE);
        }

        int version = atoi(version_str);
        if (version > 0){
            version--;
            snprintf(version_str, sizeof(version_str), "%d", version);
            if (write_config("version", version_str) < 0){
                fprintf(stderr, "Error: Could not update version.\n");
                exit(EXIT_FAILURE);
            }
            printf("Version reverted to %d.\n", version);
        } else {
            printf("Version is already 0.\n");
        }
        exit(EXIT_SUCCESS);
    }

    // 1. make filename
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

    char filename[256];
    if (!version){
        snprintf(filename, sizeof(filename), "#%s_%s_%s.zip",
            project_num, student_id, class_num);
    } else {
        snprintf(filename, sizeof(filename), "#%s_%s_%s_V%d.zip",
            project_num, student_id, class_num, version+1);
    }

    // 2. zip
    char cmd_zip[1024];
    snprintf(cmd_zip, sizeof(cmd_zip), "(cd .googit/output_dir && zip -r ../../%s .)", filename);

    if (run_command(cmd_zip) < 0){
        fprintf(stderr, "Error: Failed to create zip file.\n");
        exit(EXIT_FAILURE);
    }

    // 3. version++
    version++;
    snprintf(version_str, sizeof(version_str), "%d", version);
    if (write_config("version", version_str)){
        fprintf(stderr, "Warning: Failed to update version number in config file.\n");
    }
    
    printf("\nZip Completed: %s.\n", filename);
}