#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DRIVE "gdrive_jaeunda"
#define DIRECTORY "Ubuntu-backup"
#define PROJECT "pl-p2"

int main(void){
	char command[256];
	char timestamp[64];
	time_t now;
	struct tm *t;

	time(&now);
	t=localtime(&now);
	
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", t);
	
	// command
	snprintf(command, sizeof(command), "rclone copy ~/%s/src %s:%s/%s/%s -P", PROJECT, DRIVE, DIRECTORY, PROJECT, timestamp);

	printf("Executing command: %s\n", command);
	
	if (!system(command)) printf("Backup completed successfully.\n");
	else printf("Backup failed.\n");
	
	return 0;
}
