/* Written by Jamy Spencer 23 Feb 2017 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "slaveobj.h"

#define GO 1
#define STOP 0

int is_t1_grtr_than_t2(struct timespec t1, struct timespec t2);
struct timespec addTimeSpecs(struct timespec t1, struct timespec t2);

int main ( int argc, char *argv[] ){
	struct timespec now;
	struct timespec start;
	struct timespec run_time;
	struct timespec x_time;
	

	int doing_it = GO;

	//initiallize locking message
	msg_t my_lock;
	my_lock.mtype = 9;
	strcpy(my_lock.mtext, '\0');
	int lock_que = lockMsgMakeAttach();
	msg_t unlock;;

	//get shared memory to the system clock
	int clock_memid;
	struct timespec *sys_clock;
	sys_clock = shrMemMakeAttach(&clock_memid);
	

	//protect initial time assignment
	if((msgrcv(lock_que, &unlock, sizeof(msg_t), 9, 0)) == -1){
		perror("msgrcv");
	}
	start.tv_sec = sys_clock->tv_sec;
	start.tv_nsec = sys_clock->tv_nsec;

	if ((msgsnd(lock_que, &my_lock, sizeof(msg_t), IPC_NOWAIT)) == -1){
		perror("msgsnd");
	}	
	//end initial time protection

	//set run time
	srand(time(NULL));
	run_time.tv_sec = 0;
	run_time.tv_nsec = (rand() % 100000) + 1;

	x_time = addTimeSpecs(start, run_time);

	while (doing_it){
	//Critical Section--------------------------------------------------------
		if((msgrcv(lock_que, &unlock, sizeof(msg_t), 9, 0)) == -1){
			perror("msgrcv, slave");
		}
		
		fprintf(stderr, "Entering: %d\n", getpid());

		now.tv_sec = sys_clock->tv_sec;
		now.tv_nsec = sys_clock->tv_nsec;
//		fprintf(stderr, "Return time is: %lu %lu\n", x_time.tv_sec, x_time.tv_nsec);
//		fprintf(stderr, "Current time is: %lu %lu\n", now.tv_sec, now.tv_nsec);

		doing_it = is_t1_grtr_than_t2(x_time, now);

		if (doing_it == STOP){
			//initiallize exit message
			msg_t x_msg;
			x_msg.mtype = 1;
			
			sprintf(x_msg.mtext,"%02lu%09lu", now.tv_sec, now.tv_nsec);

			if((msgsnd(lock_que, &x_msg, sizeof(msg_t), IPC_NOWAIT)) == -1){
				perror("msgsnd: x_msg");
			}
			if((msgrcv(lock_que, &x_msg, sizeof(msg_t), 2, 0)) == -1){
				perror("msgrcv, returning");
			}
		}
		fprintf(stderr, "Exiting: %d\n", getpid());

		if ((msgsnd(lock_que, &my_lock, sizeof(msg_t), IPC_NOWAIT)) == -1){
			perror("msgsnd");
		}
	//End Critical Section---------------------------------------------------			
	}


	return 0;
}

struct timespec addTimeSpecs(struct timespec t1, struct timespec t2){
	struct timespec temp;

	temp.tv_sec = t1.tv_sec + t2.tv_sec;
	temp.tv_nsec = t1.tv_nsec + t2.tv_nsec;

	if(temp.tv_sec >= BILLION){
		temp.tv_nsec -= BILLION;
		(temp.tv_sec)++;
	}
	return temp;
}

int is_t1_grtr_than_t2(struct timespec t1, struct timespec t2){
	if (t1.tv_sec > t2.tv_sec) return 1;
	else if (t1.tv_sec < t2.tv_sec) return 0;
	else if (t1.tv_nsec > t2.tv_nsec) return 1;
	return 0;
}

	





