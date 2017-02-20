#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "slaveobj.h"

char* MakeMsg(int shared_num);
void CriticalSection(int*);
int SaveLog(char* log_file_name);


int main ( int argc, char *argv[] ){

key_t key;
int shmid;
int* shrd_data;

int i, j;
int loop_quan = atoi(argv[1]);/*
char* file_name = argv[2];
int num_slaves = atoi(argv[3]);
int id = atoi(argv[4]);
*/
perror("made it to the slave");

	/* make the key: */
    if ((key = ftok("main.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE,  0666 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    shrd_data = (int*) shmat(shmid, NULL, 0);
    if (shrd_data == (int *)(-1)) {
        perror("shmat");
        exit(1);
    }

	int* bakery_vals = shrd_data + 1;
	int* choosing = shrd_data + 16;
	int* master_num = shrd_data + 32;
	(*master_num) = 0;
	for (i = 0; i < 15; i++){
		*(bakery_vals + i) = 0;
	}

printf("Slave value of shared: %d\n", *shrd_data);
printf("in slave shared is %p\n", (int*)shrd_data);
/*
	for (i = 0; i < loop_quan; i++){
		// Bakery Algorithm
		
		choosing[id] = 1;
		bakery_vals[id] = *master_num;
		choosing[id] = 0;
		(*master_num)++;
		for (j = 0; j < num_slaves; j++){
			while(choosing[j]);
			while((bakery_vals[j] != 0) && (bakery_vals[j] < bakery_vals[id] || (bakery_vals[j] == bakery_vals[id] && j < id) ));
		}
	
		CriticalSection(shrd_data);
		bakery_vals[id] = 0;
		
	}
*/
	return 0;
}

void CriticalSection(int* shr_tot){

	srand(time(NULL));
	(*shr_tot)++;
	printf("In %d process. The total is %d\n", getpid(), (*shr_tot));
	sleep(rand() % 3);
//	SaveLog(save_file);
	sleep(rand() % 3);

}

void WriteMsg(void){

}

int SaveLog(char* log_file_name) {
	FILE* file_write = fopen(log_file_name, "a");
	char* str_out = MakeMsg(5);
	fprintf(file_write, "%s", str_out);
	free(str_out);//
	
	fclose(file_write);
	return 0;
}

char* MakeMsg(int shared_num){

	char buffer[150];
	char* msg;
	struct timespec time;

	clock_gettime(CLOCK_REALTIME, &(time));
	sprintf(buffer, "File modified by process number %d at time %lu%09lu with sharedNum %d\n", getpid(), time.tv_sec, time.tv_nsec, shared_num);
	msg = (char*) malloc (strlen(buffer) + 1);
	sprintf(msg, "%s", buffer);

	return msg;
}
