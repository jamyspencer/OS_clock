/* Written by Jamy Spencer 23 Feb 2017 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/msg.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "forkerlib.h"
#include "slaveobj.h"


void AbortProc();
void AlarmHandler();

static struct list *hd_ptr;
static struct timespec *my_clock;
static int shmid;
static int lock_que_id;
static char* file_name;

int main ( int argc, char *argv[] ){

	file_name = "test.out";
	int c;

	int num_children = 5;
	int max_run_time = 20;
	int child_count = 0;
	int total_spawned = 0;

	pid_t returning_child;

	signal(2, AbortProc);
	signal(SIGALRM, AlarmHandler);
	hd_ptr = NULL;

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

	//Initiallize clock
	struct timespec *my_clock;
	my_clock = shrMemMakeAttach(&shmid);
	my_clock->tv_nsec = 0;
	my_clock->tv_sec = 0;
		
	//set up lock queue with a message to allow the first user in
	lock_que_id = lockMsgMakeAttach();
	msg_t *my_lock;
	my_lock = malloc (sizeof(msg_t) + 4);
	(*my_lock).mtype = 3;
	strncpy((*my_lock).mtext, "main", 4);


	//set up msg_t to send acknowledgement to exiting users
	msg_t* xt_user;
	xt_user = malloc(sizeof(msg_t) + 1);
	(*xt_user).mtype = 2;
	strncpy((*xt_user).mtext, "y", 2);
	msg_t* unlock;
	unlock = malloc(sizeof(msg_t) + 11);

	hd_ptr = MakeChildren(hd_ptr, &child_count, &total_spawned, num_children);
	if (hd_ptr == NULL){
		perror("List returned NULL, exiting");
		AbortProc();
	}
	if ((msgsnd(lock_que_id, my_lock, sizeof(msg_t) + 4, 0)) == -1){
		perror("msgsnd, initial message");
	}
	do{//spin-waiting for users to return while advancing clock

		if ((returning_child = waitpid(-1, NULL, WNOHANG)) != 0){

			if (returning_child != -1){
				hd_ptr = destroyNode(hd_ptr, returning_child, file_name);
//				printf("Child %d returned/removed\n", returning_child);
				child_count--;
			}
			
			if (total_spawned < 100 && my_clock->tv_sec < 2){
				hd_ptr = MakeChildren(hd_ptr, &child_count, &total_spawned, num_children);
				if (hd_ptr == NULL){
					perror("List returned NULL, exiting");
					AbortProc();
				}
			}
		}
		//
		if((msgrcv(lock_que_id, unlock, sizeof(msg_t) + 11, 0, 0)) ==-1){
			perror("msgrcv");
		}
//		printf("Message received: %s\n", unlock->mtext);
		clock_tick(my_clock, 92000);
		if(unlock->mtype == 1){
//printf ("appending %d \n", unlock->pid);
			appendMsg(hd_ptr, my_clock, unlock->pid, unlock->mtext);
			if ((msgsnd(lock_que_id, xt_user, sizeof(msg_t) + 1, 0)) == -1){
				perror("msgsnd");
			}
		}
		else{
			if ((msgsnd(lock_que_id, my_lock, sizeof(msg_t) + 1, 0)) == -1){
				perror("msgsnd, post-clock-tick");
			}
		}

//	printf("Total Users: %d \t Active users: %d\n", total_spawned, child_count);	
	}while(child_count > 0);

	free(my_lock);
	free(unlock);
	free(xt_user);
	msgctl(lock_que_id, IPC_RMID, NULL);
	shmdt(my_clock);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}

void AlarmHandler(){
	perror("Time ran out");
	KillSlaves(hd_ptr, file_name);
	msgctl(lock_que_id, IPC_RMID, NULL);
	shmdt(my_clock);
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);
}

void AbortProc(){
//	signal(2, AbortProc);
	KillSlaves(hd_ptr, file_name);
	msgctl(lock_que_id, IPC_RMID, NULL);
	shmdt(my_clock);
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);
}

PrintList(){
	struct list * this = hd_ptr;
	
	while (this){
		printf("pid: %d\n", this->item.process_id);
		this = this->next;
	}
	printf("*****END*******\n");
}
