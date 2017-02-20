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
static int* shrd_data;
static int shmid;

int main ( int argc, char *argv[] ){

	char* file_name = "test.out";
	int c, i;

	int num_slave_processes = 4;
	char* num_increments = "3";
	int secs_until_terminate = 20;
	int slave_count = 0;


	pid_t returning_child;
	key_t key;

	signal(2, AbortProc);	

	hd_ptr = malloc(sizeof(SLV_LIST));

	//check that there is the correct number of command line arguments


	while ( (c = getopt(argc, argv, "hi:l:s:t")) != -1) {
		switch(c){
		case 'h':
			printf("-h\tHelp Menu\n-i\tChanges the number of increments each slave process does(default is 3)\n");
			printf("-l\tSet log file name(default is test.out)\n-s\tChanges the number of slave processes(default is 4)\n");
			printf("-t\tChanges the number of seconds to wait until the master terminates all slaves and itself(default is 20)\n");
			return 0;
			break;
		case 'i':
			num_increments = optarg;
			break;
		case 'l':
			file_name = optarg;
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

	    /* make the key: */
    if ((key = ftok("main.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }
//printf("Main Key: %d\n", key);
    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    shrd_data = shmat(shmid, (void*) NULL, 0);
    if (shrd_data == (int *)(-1)) {
        perror("shmat");
        exit(1);
    }

    /* read or modify the segment*/
    for (i = 0; i < MAX_SLAVES*2+2; i++){
		*(shrd_data + i) = (int)0;
	}
	*(shrd_data + MAX_SLAVES*2+2) = (int)1;

	for (i = 0; i < num_slave_processes; i++){
		hd_ptr = MakeSlave(hd_ptr, num_slave_processes, i, num_increments, file_name);
		if (hd_ptr == NULL){
			perror("Error: MakeSlave() returned NULL");
			exit(1);
		}
		else{
			slave_count++;	
		}
	}


	while(slave_count > 0){
		returning_child = wait(NULL);
		if (returning_child != -1){
			hd_ptr = destroyNode(hd_ptr, returning_child);
			printf("Child %d returned/removed\n", returning_child);
		}
		slave_count--;
	}
//	printf("The total is %d", shrd_data);
	shmdt(shrd_data);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}

void AbortProc(){
	signal(2, AbortProc);
	KillSlaves(hd_ptr);
	shmdt(shrd_data);
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);
}
