#ifndef COMMANDS_H
#define COMMANDS_H

void execute_init(const char *project_num, const char *student_id, const char *class_num);
void execute_push(const char *msg);
void execute_run(int is_clean);
void execute_zip(int is_revert);
void execute_test(const char *filename);
void execute_restore(void);

#endif