#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "commands.h"
#include "utils.h"

static int backup_to_gdrive(const char *msg);
void execute_push(const char *msg){
    char sync_command[1024];
    // 1. copy to output_dir
    snprintf(sync_command, sizeof(sync_command), 
        "rsync -a --delete --compare-dest=.googit/original/ xv6-public/ .googit/output_dir/");
    
    if (run_command(sync_command) != 0){
        fprintf(stderr, "Error: Failed to sync modified files.\n");
        exit(EXIT_FAILURE);
    }
    // 2. backup output_dir to Google Drive
    if (backup_to_gdrive(msg) < 0){
        fprintf(stderr, "Error: Failed to backup to Google Drive\n");
        exit(EXIT_FAILURE);
    }

    printf("\nPush completed.\n");
}
static int backup_to_gdrive(const char *msg){
    char drive_name[64];
    char backup_dir[64];
    char project_num[64];
    if (read_config("drive_name", drive_name, sizeof(drive_name)) < 0 ||
        read_config("backup_dir", backup_dir, sizeof(backup_dir)) < 0 ||
        read_config("project_num", project_num, sizeof(project_num)) < 0){
        fprintf(stderr, "Error: Could not read config file.\n");
        return -1;
    }

    char timestamp[64];
    time_t now;
    struct tm *t;
    time(&now);
    t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", t);

    char rclone_command[1024];
    if (msg){
        snprintf(rclone_command, sizeof(rclone_command), 
            "rclone copy .googit/output_dir %s:%s/os-p%s/%s_%s -P",
            drive_name, backup_dir, project_num, timestamp, msg);
    } else {
        snprintf(rclone_command, sizeof(rclone_command), 
            "rclone copy .googit/output_dir %s:%s/os-p%s/%s -P",
            drive_name, backup_dir, project_num, timestamp);
    }

    return run_command(rclone_command);
}