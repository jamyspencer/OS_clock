/* Written by Jamy Spencer 23 Feb 2017 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h> 
#include <time.h>
#include <unistd.h>
#include "forkerlib.h"
#include "slaveobj.h"



SLV_LIST* MakeChildren(SLV_LIST* head_ptr, int* child_count, int* total_spawned, int num_children){

	pid_t pid;

	while(*child_count < num_children){

		pid = fork();
		if (pid < 0){
			perror("Fork failed");
			return NULL;
		}
		else if (pid > 0){			
			if (head_ptr->item.process_id == 0){
				head_ptr->item.process_id = pid;
			}
			else{
				addNode(head_ptr, pid);
//printf("Total: %d here: %d\n", *total_spawned, *child_count);
			}
			(*child_count)++;
			(*total_spawned)++;
		}
		else if (pid == 0){
			execl("./user", "user", (char*) NULL);
		}
		
	}
	return head_ptr;
}

void KillSlaves(SLV_LIST* head_ptr){
	while (head_ptr != NULL){	
		kill(head_ptr->item.process_id, SIGKILL);
		head_ptr = destroyHead(head_ptr);	
	}
}

int SaveLog(char* log_file_name, char* msg) {
	FILE* file_write = fopen(log_file_name, "a");

	fprintf(file_write, "%s", msg);
	fclose(file_write);
	return 0;
}

char* MakeMsg(int* shr_tot){

	char buffer[150];
	char* msg;
	struct timespec time;

	clock_gettime(CLOCK_REALTIME, &(time));
	sprintf(buffer, "File modified by process number %d at time %lu%09lu with sharedNum %d\n", getpid(), time.tv_sec, time.tv_nsec, *shr_tot);
	msg = (char*) malloc (strlen(buffer) + 1);
	sprintf(msg, "%s", buffer);

	return msg;
}

void clock_tick(struct timespec *clock, int increment){

	while (increment){
		if (clock->tv_nsec < (BILLION - increment)){
			(clock->tv_nsec) += increment;
			increment = 0;
		}
		else{
			(clock->tv_sec)++;
			clock->tv_nsec = ((clock->tv_nsec) + increment - BILLION);
		}
	}
//	fprintf(stderr, "SYSTEM CLOCK: %lu %lu\n", clock->tv_sec, clock->tv_nsec);
}

