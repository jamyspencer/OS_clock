#include <stdlib.h>
#include <stdio.h>
#include  <sys/types.h>
#include "slavelib.h"

int main ( int argc, char *argv[] ){

	struct slave_info this_info;

	this_info.process_id = getpid();

	CriticalSection();
}
