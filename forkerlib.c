/* Written by Jamy Spencer 30 Jan 2017 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h> 
#include <time.h>
#include "forkerlib.h"
#include "slaveobj.h"


SLV_LIST* MakeSlave(SLV_LIST* head_ptr, int num_slave_processes, int i, char* num_increments, char* file_name){
printf("The num is %d\n", i);
printf("the other num is %d\n", num_slave_processes);
	pid_t pid;
	char* num_slaves;
	char* id;
	sprintf(num_slaves, "%d", num_slave_processes);
	sprintf(id, "%d", i);

	pid = fork();
	if (pid < 0){
		errno = ECHILD;
		perror("Fork failed");
		KillSlaves(head_ptr);
		return 1;
	}
	else if (pid > 0){			
		if (head_ptr->item.process_id == 0){
			head_ptr->item.process_id = pid;
//				printf("PID of HEAD is: %d\n", head_ptr->item.process_id);
//				printf("Address of HEAD is: %d\n", head_ptr);
		}
		else{
			addNode(head_ptr, pid);
		}
	}
	else if (pid == 0){
printf("made it to child");
		

printf ("%s\n", num_slaves);
printf ("%s\n", id);
//perror("A slave has been born");
		execl("./slave", "slave", num_increments, file_name, num_slaves, id, (char*) NULL);	
	}
//	printf("PID of HEAD is: %d\n", head_ptr->item.process_id);
	return head_ptr;
}

void KillSlaves(SLV_LIST* head_ptr){
	while (head_ptr != NULL){	
		kill(head_ptr->item.process_id, SIGKILL);
		head_ptr = destroyHead(head_ptr);	
	}
	//perror("Master killed the slaves.");
}



