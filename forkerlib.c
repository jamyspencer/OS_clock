/* Written by Jamy Spencer 30 Jan 2017 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "forkerlib.h"
#include "slaveobj.h"


SLV_LIST* MakeSlaves(int num_slaves, SLV_LIST* hd_ptr){

	pid_t pid;
	int i;
	SLV_LIST* temp;

	for (i = 0; i < num_slaves; i++){
		pid = fork();
		if (pid < 0){
			errno = ECHILD;
			perror("Fork failed");
			KillSlaves(hd_ptr);
			return 1;
		}
		else if (pid > 0){
//			perror("A slave has been born");
			if (hd_ptr->item.process_id == 0){
				hd_ptr->item.process_id = pid;
//				printf("PID of HEAD is: %d\n", hd_ptr->item.process_id);
//				printf("Address of HEAD is: %d\n", hd_ptr);
			}
			else{
				temp = hd_ptr;
				while (temp->next != NULL){
					temp = temp->next;
				}
				temp->next = malloc (sizeof(SLV_LIST));
				temp = temp->next;
				temp->item.process_id = pid;

			}
		}
		else if (pid == 0){
			execl("./", "./slave");	
//			sleep(1000);
		}
	}
//	printf("PID of HEAD is: %d\n", hd_ptr->item.process_id);
	return hd_ptr;
}

void KillSlaves(SLV_LIST* hd_ptr){
	SLV_LIST* temp;

	while (hd_ptr != NULL){
//		printf("PID of HEAD is: %d\n", hd_ptr->item.process_id);
		temp = hd_ptr;		
		hd_ptr = hd_ptr->next;
//		printf("The process id is: %d\n", temp->item.process_id);
		kill(temp->item.process_id, SIGKILL);
		free(temp);	
	}
	perror("Master killed the slaves.");
}

