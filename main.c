/* Written by Jamy Spencer 06 Feb 2017 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/types.h>
#include <unistd.h>
#include "forkerlib.h"
#include "slaveobj.h"

void AbortProc();

static SLV_LIST* hd_ptr;
static int* shm_addr;

int main ( int argc, char *argv[] ){

	const key_t KEY = 9876;
	const int MAX_SLAVES = 15;

	char* log_file_name = "test.out";
	int c, i;
	int num_slave_processes = 5;
	int num_increments = 3;
	int secs_until_terminate = 20;
	int slave_count = 0;

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
		if (MakeSlave(hd_ptr) == NULL){
			perror("Error: MakeSlave() returned NULL");
			exit(1);
		}
		else{
			slave_count++;	
		}
	}

	shm_addr = AllocateSharedMemory(KEY);

sleep (10);	
/*
	for (i = 0; i < num_slave_processes; i++){
		 wait();
	}		
*/
	
	KillSlaves(hd_ptr);

	return 0;
}

void AbortProc(){
	signal(2, AbortProc);
	KillSlaves(hd_ptr);
	printf("In the abort function\n");
	shmdt(shm_addr);
	exit(1);
}
