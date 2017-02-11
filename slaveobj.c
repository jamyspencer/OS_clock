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
			if (temp == head_ptr){
				return destroyHead(head_ptr);
			}
			else if (temp->next == NULL){
				(temp->prev)->next = NULL;
				free(temp);
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
}

struct list* destroyHead(struct list *head_ptr){
	struct list *temp = head_ptr;	
	head_ptr = head_ptr->next;
	if (head_ptr != NULL){
		free(temp);
		head_ptr->prev = NULL;
	}
	return head_ptr;
}

