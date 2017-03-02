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

struct list *MakeChildren(struct list *head_ptr, int* child_count, int* total_spawned, int num_children){
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
	while(temp != NULL && temp->next){
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
		temp = temp->next;
	}
	return NULL;
}

void appendMsg(struct list* head_ptr, struct timespec *clock, pid_t pid, char* done_time){
	struct list *done = findNodeByPid(head_ptr, pid);
	if (done){
		done->item.msg = MakeMsg(clock, done_time, pid);//malloc'd
	}
	else{
		perror("Could not find pid");
	}
	return;
}

char* MakeMsg(struct timespec* clock, char* x_time, pid_t pid){

	char buffer[110];
	char* msg;

	snprintf(buffer, 110, "Master: Child(%d) is terminating at my time %02lu%09lu because it reached %s in slave\n", pid, clock->tv_sec, clock->tv_nsec, x_time);
	buffer[109] = '\0';
	msg = (char*) malloc (strlen(buffer));
	sprintf(msg, "%s", buffer);
//	printf("%s", msg);
	return msg;//malloc'd
}

int SaveLog(char* log_file_name, char* msg, pid_t pid) {
	FILE* file_write = fopen(log_file_name, "a");

	if (msg == NULL){
		fprintf(file_write, "Process: %d shut down abnormally.", pid);
	}
	else{
		fprintf(file_write, "%s", msg);
	}
	fclose(file_write);
	return 0;
}

//returns the head_ptr address of the list that now has the node containing the pid passed removed
struct list* destroyNode(struct list *head_ptr, pid_t pid, char* file_name){
	struct list *temp = findNodeByPid(head_ptr, pid);
	struct list *new_head;

	SaveLog(file_name, temp->item.msg, temp->item.process_id);
	if (temp == NULL){
		printf("Couldn\'t find %d ", pid);
		return head_ptr;
	}
	else if (temp == head_ptr){//delete head
		if(temp){
			new_head = temp->next;//set new head to next
			if (new_head){//set the new heads prev to NULL
				new_head->prev = NULL;
			}
			free(temp->item.msg);
			free(temp);
			temp = NULL;		
		}
		else{perror("attempted to destroy null head");}
		return new_head;
	}
	else if (temp->next == NULL){//delete tail

		free(temp->item.msg);
		(temp->prev)->next = NULL;
		free(temp);
		return head_ptr;
	}
	else{//delete a mid

		(temp->prev)->next = temp->next;
		(temp->next)->prev = temp->prev;
		free(temp->item.msg);
		free(temp);
		return head_ptr;
	}
	return NULL;
}

void KillSlaves(struct list *head_ptr, char* file_name){
	while (head_ptr != NULL){	
		kill(head_ptr->item.process_id, SIGKILL);
		destroyNode(head_ptr, (head_ptr->item).process_id, file_name);	
	}
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
