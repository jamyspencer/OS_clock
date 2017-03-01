/* Written by Jamy Spencer 23 Feb 2017 */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/types.h>
#include <sys/msg.h>
#include <time.h>
#include "slaveobj.h"
#include <unistd.h>




struct timespec* shrMemMakeAttach(int* shmid){

	struct timespec *shrd_data;
	key_t key;

	/* make the key: */
    if ((key = ftok("main.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((*shmid = shmget(key, sizeof(struct timespec), IPC_CREAT | 0644)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    shrd_data = shmat(*shmid, (void*) NULL, 0);
    if (shrd_data == (void *)(-1)) {
        perror("shmat");
        exit(1);
    }
	return shrd_data;
}

int lockMsgMakeAttach(void){

	int msgque;
	key_t key;

	/* make the key: */
    if ((key = ftok("main.c", 'X')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((msgque = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget");
        exit(1);
    }	
	return msgque;
}

