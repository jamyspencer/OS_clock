#include <signal.h>
#include <stdlib.h>
#include "slaveobj.h"

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
}

//returns the head_ptr address of the list that now has the node containing the pid passed removed
struct list* destroyNode(struct list *head_ptr, pid_t pid){
	struct list *temp = head_ptr;
	while(temp->next != NULL){
		if (temp->item.process_id == pid){
			kill((temp)->item.process_id, SIGKILL);
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
	}
	return NULL;
}

struct list* destroyHead(struct list *head_ptr){
	struct list *temp = head_ptr;	
	
	if (temp != NULL){
		head_ptr = head_ptr->next;
		kill((temp)->item.process_id, SIGKILL);
		printf("%d killed\n", (temp)->item.process_id);
		free(temp);
		if (head_ptr != NULL){
			head_ptr->prev = NULL;
		}
	}
	return head_ptr;
}

