/* Written by Jamy Spencer 06 Feb 2017 */
#include <stdio.h>
#include "log.h"
#include <unistd.h>

int main ( int argc, char *argv[] ){

	char* log_file_name = "logfile.txt";
	int c;
	int num_slave_processes = 5;
	int num_increments = 3;
	int secs_until_terminate = 20;

	//check that there is the correct number of command line arguments

	while ( (c = getopt(argc, argv, "hil:s:t")) != -1) {
		switch(c){
		case 'h':
			printf("%s\n", "-h\tHelp Menu\n-n\tSet special number\n-l\tSet log file name");
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


	return 0;
}
