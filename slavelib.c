#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include  <sys/types.h>
#include "slavelib.h"

void CriticalSection(void){
	srand(time(NULL));

	sleep(rand() % 3);
	SaveLog();
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
	sprintf(buffer, "File modified by process number %d at time %lu%09lu with sharedNum %s\n", getpid(), time.tv_sec, time.tv_nsec, shared_num);
	msg = (char*) malloc (strlen(buffer) + 1);
	sprintf(msg, "%s", buffer);

	return msg;
}
