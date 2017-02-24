/* Written by Jamy Spencer 23 Feb 2017 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "slaveobj.h"
#include "ossclock.h"


void CriticalSection(char*, int*);
int SaveLog(char* log_file_name, char*);


int main ( int argc, char *argv[] ){

int* oss_clock;



perror("made it to the slave");

//	oss_clock = shrMemMakeAttach();


			

	return 0;
}

void CriticalSection(char* file_name, int* shr_tot){

	fprintf(stderr, "Entering Critical Section: %d: %d\n", getpid(), (*shr_tot));

	srand(time(NULL));
//	sleep(rand() % 3); // sleep between 0 and 2 secs


//	sleep(rand() % 3);
	perror ("Exiting Critical Section");

}



