/* Written by Jamy Spencer 06 Feb 2017 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "forkerlib.h"
#include "slaveobj.h"

void AbortProc();

static SLV_LIST* hd_ptr;
static int* shared_total;
static int mem_id;

int main ( int argc, char *argv[] ){

	const int MAX_SLAVES = 15;

	char* log_file_name = "test.out";
	int c, i;
	int num_slave_processes = 5;
	int num_increments = 3;
	int secs_until_terminate = 20;
	int slave_count = 0;
	pid_t returning_child;

	signal(2, AbortProc);	

	hd_ptr = malloc(sizeof(SLV_LIST));

	//check that there is the correct number of command line arguments


	while ( (c = getopt(argc, argv, "hi:l:s:t")) != -1) {
		switch(c){
		case 'h':
			printf("-h\tHelp Menu\n-i\tChanges the number of increments each slave process does(default is 3)\n");
			printf("-l\tSet log file name(default is test.out)\n-s\tChanges the number of slave processes(default is 5)\n");
			printf("-t\tChanges the number of seconds to wait until the master terminates all slaves and itself(default is 20)\n");
			return 0;
			break;
		case 'i':
			num_increments = atoi(optarg);
			break;
		case 'l':
			log_file_name = optarg;
			break;
		case 's':
			num_slave_processes = atoi(optarg);
			break;
		case 't':
			secs_until_terminate = atoi(optarg);
			break;
		case '?':
			return 1;
			break;
		}
	}
	for (i = 0; i < num_slave_processes; i++){
		hd_ptr = MakeSlave(hd_ptr);
		if (hd_ptr == NULL){
			perror("Error: MakeSlave() returned NULL");
			exit(1);
		}
		else{
			slave_count++;	
		}
	}

	mem_id = AllocateSharedMemory(shared_total);


	while(slave_count > 0){
		returning_child = wait(NULL);
	printf("%d\n", returning_child);
		if (returning_child != -1){
			hd_ptr = destroyNode(hd_ptr, returning_child);
		}
		slave_count--;
	}

	shmdt(shared_total);
	shmctl(mem_id, IPC_RMID, NULL);
	return 0;
}

void AbortProc(){
	signal(2, AbortProc);
	KillSlaves(hd_ptr);
	shmdt(shared_total);
	shmctl(mem_id, IPC_RMID, NULL);
	exit(1);
}
