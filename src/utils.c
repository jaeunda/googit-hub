#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "utils.h"

#define CONFIG_PATH ".googit/googit_config"
#define LINE_MAX 32
#define LEN_MAX 128
#define CONFIG_SIZE_MAX 2048

int run_command(const char *command){
    printf("Executing: %s\n", command);
    int status;
    if ((status = system(command)) != 0){
        fprintf(stderr, "Error: Command failed with status %d\n", status);
    }
    return status;
}
int read_config(const char *key, char *value_buffer, size_t buffer_size){
    int fd;
    if ((fd = open(CONFIG_PATH, O_RDONLY)) < 0){
        fprintf(stderr, "Error: cannot open config file '%s'.\n", CONFIG_PATH);
        return -1;
    }

    char buffer[CONFIG_SIZE_MAX];
    ssize_t read_bytes;
    if (lseek(fd, (off_t)0, SEEK_SET) < 0){
        perror("config");
        close(fd);
        return -1;
    }
    lock_file(fd, F_RDLCK);
    if ((read_bytes = read(fd, buffer, sizeof(buffer)-1)) < 0){
        fprintf(stderr, "Error: config read error\n");
        close(fd);
        return -1;
    }
    lock_file(fd, F_UNLCK);
    close(fd);
    buffer[read_bytes] = '\0';

    short found = 0;
    char *config_line = strtok(buffer, "\n");
    while (config_line){
        char temp_line[LEN_MAX];
        strncpy(temp_line, config_line, (size_t)(LEN_MAX-1));
        temp_line[LEN_MAX-1] = '\0';

        if (!strncmp(temp_line, key, strlen(key))){
            strtok(temp_line, "=");
            char *cur_value = strtok(NULL, "");
            strncpy(value_buffer, cur_value, (size_t)(buffer_size-1));
            value_buffer[buffer_size-1]='\0';
            found = (short) 1;
            break;
        }
        config_line = strtok(NULL, "\n");
    }
    return found ? 0 : -1;
}
int write_config(const char *key, const char *value){
    char config_lines[LINE_MAX][LEN_MAX];
    int line_count = 0;
    short key_found = 0;

    int fd;
    if ((fd = open(CONFIG_PATH, O_RDONLY | O_CREAT, 0644)) < 0){
        fprintf(stderr, "Error: config open error\n");
        return -1;
    }

    char buffer[CONFIG_SIZE_MAX];
    ssize_t read_bytes;
    if (lseek(fd, (off_t)0, SEEK_SET) < 0){
        perror("config");
        close(fd);
        return -1;
    }
    lock_file(fd, F_RDLCK);
    if ((read_bytes = read(fd, buffer, sizeof(buffer)-1)) < 0){
        fprintf(stderr, "Error: config read error\n");
        close(fd);
        return -1;
    }
    lock_file(fd, F_UNLCK);
    close(fd);
    buffer[read_bytes] = '\0';
    
    // lines
    char *temp_line = strtok(buffer, "\n");
    while (temp_line && line_count < LINE_MAX){
        strncpy(config_lines[line_count++], temp_line, (size_t)(LEN_MAX-1));
        config_lines[line_count-1][LEN_MAX-1] = '\0';
        temp_line = strtok(NULL, "\n");
    }

    // update
    for (int i=0; i<line_count; i++){
        if (!strncmp(config_lines[i], key, strlen(key)) && config_lines[i][strlen(key)] == '='){
            snprintf(config_lines[i], (size_t)LEN_MAX, "%s=%s", key, value);
            key_found = (short)1;
            break;
        }
    }
    if (!key_found && line_count < LINE_MAX){
        snprintf(config_lines[line_count++], (size_t)LEN_MAX, "%s=%s", key, value);
    }

    // write
    if ((fd = open(CONFIG_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
        fprintf(stderr, "Error: config open error\n");
        return -1;
    }
    if (lseek(fd, (off_t)0, SEEK_SET) < 0){
        perror("config");
        close(fd);
        return -1;
    }

    ssize_t write_bytes = 0;
    lock_file(fd, F_WRLCK);
    for (int i=0; i<line_count; i++){
        write_bytes += write(fd, config_lines[i], strlen(config_lines[i]));
        write_bytes += write(fd, "\n", (size_t)1);
        if (write_bytes > (ssize_t)CONFIG_SIZE_MAX){
            break;
        }
    }
    lock_file(fd, F_UNLCK);
    // errno == EINTR
    close(fd);
    return 0;
}
void setup_gdrive(void){
    printf("\n--- Starting Google Drive Setup via rclone ---\n");
    printf("1. The `rclone` configuration tool will now start.\n");
    printf("2. A browser window may open to authenticate with your Google account.\n");
    printf("3. Follow the on-screen prompts carefully:\n");
    printf("   - Choose 'n' for a new remote.\n");
    printf("   - Give your remote a simple name (e.g., 'gdrive_os'). You will need this name next.\n");
    printf("   - Choose 'drive' for Google Drive (find it in the list).\n");
    printf("   - For most other prompts, pressing Enter for the default is safe.\n");
    printf("Press Enter to continue...");

    getchar();

    run_command("rclone config");

    char drive_name[64];
    char backup_dir[64];

    printf("\n--- Googit Configuration ---\n");
    printf("Please enter the rclone remote name for Google Drive (e.g., gdrive_os): ");
    fgets(drive_name, sizeof(drive_name), stdin);
    drive_name[strcspn(drive_name, "\n")] = 0;

    printf("Enter a directory name for your backups within Google Drive (e.g., os_backup): ");
    fgets(backup_dir, sizeof(backup_dir), stdin);
    backup_dir[strcspn(backup_dir, "\n")] = 0;

    if (write_config("drive_name", drive_name) != 0 ||
        write_config("backup_dir", backup_dir) != 0) {
        fprintf(stderr, "Error: Failed to save Google Drive configuration.\n");
        exit(1);
    }

    printf("\nGoogle Drive configuration saved successfully.\n");
    return;
}
void lock_file(int fd, short lock_type){
    struct flock fl = {0};
    fl.l_type = lock_type;
    fl.l_whence = SEEK_SET;
    fl.l_start = (off_t)0;
    fl.l_len = (off_t)0;
    if (fcntl(fd, F_SETLKW, &fl) < 0){
        perror("lock_file");
        close(fd);
        exit(EXIT_FAILURE);
    }
}