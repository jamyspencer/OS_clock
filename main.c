/* Written by Jamy Spencer 06 Feb 2017 */
#include <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include "forkerlib.h"
#include "slaveobj.h"

int main ( int argc, char *argv[] ){

	char* log_file_name = "test.out";
	int c, x;
	int num_slave_processes = 5;
	int num_increments = 3;
	int secs_until_terminate = 20;



	SLV_LIST* hd_ptr = malloc(sizeof(SLV_LIST));

	//check that there is the correct number of command line arguments

	while ( (c = getopt(argc, argv, "hi:l:s:t")) != -1) {
		switch(c){
		case 'h':
			printf("-h\tHelp Menu\n-i\tChanges the number of increments each slave process does(default is 3)\n");
			printf("-l\tSet log file name(default is test.out)\n-s\tChanges the number of slave processes(default is 5)\n");
			printf("-t\tChanges the number of seconds to wait until the master terminates all slaves and itself(default is 20)\n");
			return 0;
			break;
		case 'i':
			num_increments = atoi(optarg);
			break;
		case 'l':
			log_file_name = optarg;
			break;
		case 's':
			num_slave_processes = atoi(optarg);
			break;
		case 't':
			secs_until_terminate = atoi(optarg);
			break;
		case '?':
			return 1;
			break;
		}
	}
	MakeSlaves(num_slave_processes, hd_ptr);

	
	KillSlaves(hd_ptr);

	return 0;
}
