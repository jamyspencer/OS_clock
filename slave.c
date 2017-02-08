#include <stdlib.h>
#include <stdio.h>
#include  <sys/types.h>
#include "slavelib.h"
#include "slaveobj.h"

int main ( int argc, char *argv[] ){

	SLV_INFO this_info;
	
	this_info.process_id = getpid();
	printf("booyah, in the child\n");
//	CriticalSection();

	return 0;
}
