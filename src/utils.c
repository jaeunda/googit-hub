#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "utils.h"

#define CONFIG_PATH ".googit/.googit_config"
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
        exit(1);
    }

    char buffer[CONFIG_SIZE_MAX];
    ssize_t read_bytes;
    if (lseek(fd, (off_t)0, SEEK_SET) < 0){
        perror("config");
        close(fd);
        exit(EXIT_FAILURE);
    }
    lock_file(fd, F_RDLCK);
    if ((read_bytes = read(fd, buffer, sizeof(buffer)-1)) < 0){
        fprintf(stderr, "Error: config read error\n");
        close(fd);
        exit(EXIT_FAILURE);
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

        char *cur_key = strtok(temp_line, "=");
        char *cur_value = strtok(NULL, "");
        if (cur_key && cur_value){
            if (!strcmp(cur_key, key)){
                strncpy(value_buffer, cur_value, (size_t)(buffer_size-1));
                value_buffer[buffer_size-1]='\0';
                found = (short)1;
                break;
            }
        }
        config_line = (NULL, "\n");
    }
    return found ? 0 : -1;
}
int write_config(const char *key, const char *value){
    char config_lines[LINE_MAX][LEN_MAX];
    int line_count = 0;
    short key_found = 0;

    int fd;
    if ((fd = open(CONFIG_PATH, O_RDONLY)) < 0){
        fprintf(stderr, "Error: config open error\n");
        exit(EXIT_FAILURE);
    }

    char buffer[CONFIG_SIZE_MAX];
    ssize_t read_bytes;
    if (lseek(fd, (off_t)0, SEEK_SET) < 0){
        perror("config");
        close(fd);
        exit(EXIT_FAILURE);
    }
    lock_file(fd, F_RDLCK);
    if ((read_bytes = read(fd, buffer, sizeof(buffer)-1)) <= 0){
        fprintf(stderr, "Error: config read error\n");
        close(fd);
        exit(EXIT_FAILURE);
    }
    lock_file(fd, F_UNLCK);
    close(fd);
    buffer[read_bytes] = '\0';
    
    // lines
    char *temp_line = strtok(buffer, "\n");
    while (temp_line && line_count < LINE_MAX){
        strncpy(config_lines[line_count++], temp_line, (size_t)(LEN_MAX-1));
        config_lines[line_count-1][LEN_MAX-1] = '\0';
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
        exit(EXIT_FAILURE);
    }
    if (lseek(fd, (off_t)0, SEEK_SET) < 0){
        perror("config");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t write_bytes = 0;
    lock_file(fd, F_WRLCK);
    for (int i=0; i<line_count; i++){
        write_bytes += write(fd, buffer, strlen(buffer));
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