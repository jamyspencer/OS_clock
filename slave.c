#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "slaveobj.h"

char* MakeMsg(int shared_num);
void CriticalSection(int*);
int SaveLog(char* log_file_name);


int main ( int argc, char *argv[] ){
int i;
for (i = 0; i<5; i++){
	if (argv[i] != NULL){
		printf("Slave argument %d is %s\n", i, argv[i]);
	}

}

perror("blah");
	int shmid = atoi(argv[1]);
//	key_t key = 81380;
	int* shrd_data;

    /* attach to the segment to get a pointer to it: */
    shrd_data = shmat(shmid, NULL, 0);
    if (shrd_data == (int *)(-1)) {
        perror("shmat");
        exit(1);
    }

printf("in slave shared is %d", (int)*shrd_data);
	CriticalSection(shrd_data);

	return 0;
}

void CriticalSection(int* shr_tot){
	perror("another blah");	
	srand(time(NULL));
	(*shr_tot)++;
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
