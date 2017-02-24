/* Written by Jamy Spencer 23 Feb 2017 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "forkerlib.h"
#include "slaveobj.h"
#include "ossclock.h"

void AbortProc();
void AlarmHandler();

static SLV_LIST* hd_ptr;
static int* my_clock;
static int shmid;

int main ( int argc, char *argv[] ){

	char* file_name = "test.out";
	int c, i;

	int num_children = 5;
	int max_run_time = 20;
	int child_count = 0;
	int total_spawned = 0;

	pid_t returning_child;


	signal(2, AbortProc);
	signal(SIGALRM, AlarmHandler);

	hd_ptr = malloc(sizeof(SLV_LIST));


	while ( (c = getopt(argc, argv, "hi:l:s:t:")) != -1) {
		switch(c){
		case 'h':
			printf("-h\tHelp Menu\n");
			printf("-l\tSet log file name(default is test.out)\n-s\tChanges the number of slave processes(default is 4)\n");
			printf("-t\tChanges the number of seconds to wait until the master terminates all slaves and itself(default is 20)\n");
			return 0;
			break;
		case 'l':
			file_name = optarg;
			break;
		case 's':
			num_children = atoi(optarg);
			if (num_children > MAX_USERS){
				printf("Error: -s exceeds MAX_USERS, set to %d\n", MAX_USERS);
			}
			break;
		case 't':
			max_run_time = atoi(optarg);
			break;
		case '?':
			return 1;
			break;
		}
	}
	alarm(max_run_time);

	my_clock = shrMemMakeAttach(&shmid);

	hd_ptr = MakeChildren(hd_ptr, &child_count, &total_spawned, num_children);
	if (hd_ptr == NULL){
		perror("List returned NULL, exiting");
		AbortProc();
	}


	while(total_spawned < 100 && *my_clock < 2){
		
		if (waitpid(-1, NULL, WNOHANG) != 0){
			returning_child = wait(NULL);
			if (returning_child != -1){
				hd_ptr = destroyNode(hd_ptr, returning_child);
				printf("Child %d returned/removed\n", returning_child);
			}
			child_count--;
			hd_ptr = MakeChildren(hd_ptr, &child_count, &total_spawned, num_children);
			if (hd_ptr == NULL){
				perror("List returned NULL, exiting");
				AbortProc();
			}
		}
		clock_tick(my_clock, 500);

	}

	shmdt(my_clock);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}

void AlarmHandler(){
	perror("Time ran out");
	KillSlaves(hd_ptr);
	shmdt(my_clock);
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);
}

void AbortProc(){
//	signal(2, AbortProc);
	KillSlaves(hd_ptr);
	shmdt(my_clock);
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);
}
