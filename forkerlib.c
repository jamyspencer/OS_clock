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
			head_ptr = addNode(head_ptr, pid);
			(*child_count)++;
			(*total_spawned)++;
		}
		else if (pid == 0){
			execl("./user", "user", (char*) NULL);
		}	
	}
	return head_ptr;
}

struct list *addNode(struct list *head_ptr, pid_t pid){
	struct list *temp = malloc (sizeof(struct list));
	temp->item.msg = NULL;
	temp->item.process_id = pid;
	temp->next = NULL;

	if (head_ptr == NULL){
		temp->prev = NULL;
		return temp;
	}
	else{
	struct list *tail = returnTail(head_ptr);
	tail->next = temp;
	temp->prev = tail;
	}
	return head_ptr;
}

struct list *returnTail(struct list *head_ptr){
	struct list *temp = head_ptr;
	while(temp != NULL && temp->next != NULL){
		temp = temp->next;	
	}
	return temp;
}

struct list* findNodeByPid(struct list *head_ptr, pid_t pid){
	struct list *temp = head_ptr;
	while(temp != NULL){
		if (temp->item.process_id == pid){
			return temp;
		}
		else{
			temp = temp->next;
		}
	}
	return NULL;
}

//returns the head_ptr address of the list that now has the node containing the pid passed removed
struct list* destroyNode(struct list *head_ptr, pid_t pid){
	struct list *temp = findNodeByPid(head_ptr, pid);
	
	if (temp == NULL){
		printf("Couldn\'t find %d ", pid);
		return head_ptr;
	}
	else if (temp == head_ptr){
		destroyHead(head_ptr);
		return head_ptr;
	}
	else if (temp->next == NULL){
		free(temp);
		(temp->prev)->next = NULL;
		return head_ptr;
	}
	else{
		(temp->prev)->next = temp->next;
		(temp->next)->prev = temp->prev;
		free(temp);
		return head_ptr;
	}
	return NULL;
}

void destroyHead(struct list *head_ptr){
	struct list *temp = head_ptr;	
	
	if (temp != NULL){
		head_ptr = head_ptr->next;
		free(temp);
		temp = NULL;
		if (head_ptr != NULL){
			head_ptr->prev = NULL;
		}
	}
	return;
}

void KillSlaves(SLV_LIST* head_ptr){
	while (head_ptr != NULL){	
		kill(head_ptr->item.process_id, SIGKILL);
		destroyHead(head_ptr);	
	}
}

int SaveLog(char* log_file_name, char* msg) {
	FILE* file_write = fopen(log_file_name, "a");

	fprintf(file_write, "%s", msg);
	fclose(file_write);
	return 0;
}

char* MakeMsg(struct timespec* clock, char* x_time){

	char buffer[110];
	char* msg;

	snprintf(buffer, 110, "Master: Child pid is terminating at my time %02lu%09lu because it reached %s in slave", clock->tv_sec, clock->tv_nsec, x_time);
	buffer[109] = '\0';
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
			clock->tv_nsec = ((clock->tv_nsec)	 + increment - BILLION);
		}
	}
//	fprintf(stderr, "SYSTEM CLOCK: %lu %lu\n", clock->tv_sec, clock->tv_nsec);
}



