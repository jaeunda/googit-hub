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
        "rsync -a --delete --compare-dest=.googit/original/ xv6-public/ .googit_output_dir/");
    
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