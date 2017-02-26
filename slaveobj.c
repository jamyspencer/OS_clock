/* Written by Jamy Spencer 23 Feb 2017 */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/msg.h>
#include <time.h>
#include "slaveobj.h"
#include <unistd.h>


struct list *returnTail(struct list *head_ptr){
	struct list *temp = head_ptr;
	while(temp->next != NULL){
		temp = temp->next;	
	}
	return temp;
}

void addNode(struct list *head_ptr, pid_t pid){
	struct list *tail = returnTail(head_ptr);
	tail->next = malloc (sizeof(struct list));
	(tail->next)->prev = tail;
	(tail->next)->item.process_id = pid;
	(tail->next)->next = NULL;
}

//returns the head_ptr address of the list that now has the node containing the pid passed removed
struct list* destroyNode(struct list *head_ptr, pid_t pid){
	struct list *temp = head_ptr;
	while(temp != NULL && temp->next != NULL){
		if (temp->item.process_id == pid){
			if (temp == head_ptr){
				return destroyHead(head_ptr);
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
		}
		else{
			temp = temp->next;
		}
	}
	return NULL;
}

struct list* destroyHead(struct list *head_ptr){
	struct list *temp = head_ptr;	
	
	if (temp != NULL){
		head_ptr = head_ptr->next;
		free(temp);
		temp = NULL;
		if (head_ptr != NULL){
			head_ptr->prev = NULL;
		}
	}
	return head_ptr;
}

struct timespec* shrMemMakeAttach(int* shmid){

	struct timespec *shrd_data;
	key_t key;

	/* make the key: */
    if ((key = ftok("main.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((*shmid = shmget(key, sizeof(struct timespec), IPC_CREAT | 0644)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    shrd_data = shmat(*shmid, (void*) NULL, 0);
    if (shrd_data == (void *)(-1)) {
        perror("shmat");
        exit(1);
    }
	return shrd_data;
}

int lockMsgMakeAttach(void){

	int msgque;
	key_t key;

	/* make the key: */
    if ((key = ftok("main.c", 'X')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((msgque = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget");
        exit(1);
    }	
	return msgque;
}

