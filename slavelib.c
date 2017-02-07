#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "slavelib.h"

void CriticalSection(void){
srand(time(NULL));

sleep(rand() % 3);

sleep(rand() % 3);

}

void WriteMsg(void){

}

int SaveLog(char* log_file_name) {
	FILE* file_write = fopen(log_file_name, "a");
	char* str_out = MakeMsg();
	fprintf(file_write, "%s", str_out);
	free(str_out);//
	
	fclose(file_write);
	return 0;
}

char* MakeMsg(void){

	char buffer[200];
	struct time_t time;

	clock_gettime(CLOCK_REALTIME, &(time);
	sprintf(buffer, "File modified by process number %d at time %lu%09lu with sharedNum %s\n", process_id, temp.time.tv_sec, temp.time.tv_nsec, spec_num);
	temp.err_msg = (char*) malloc (strlen(buffer) + 1);
	sprintf(temp.err_msg, "%s", buffer);
//	printf("%s\n", temp.err_msg);
	return temp;
}
