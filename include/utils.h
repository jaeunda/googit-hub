#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

int run_command(const char *command);
int read_config(const char *key, char *value_buffer, size_t buffer_size);
int write_config(const char *key, const char *value);

void lock_file(int fd, short lock_type);

#endif