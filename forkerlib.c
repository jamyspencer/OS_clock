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
				addNode(hd_ptr, pid);
			}
		}
		else if (pid == 0){
			execl("./slave", (char*)0);	
//			sleep(1000);
		}
	}
//	printf("PID of HEAD is: %d\n", hd_ptr->item.process_id);
	return hd_ptr;
}

void KillSlaves(SLV_LIST* hd_ptr){
	while (hd_ptr != NULL){	
		kill((hd_ptr)->item.process_id, SIGKILL);
		hd_ptr = destroyHead(hd_ptr);	
	}
	perror("Master killed the slaves.");
}

