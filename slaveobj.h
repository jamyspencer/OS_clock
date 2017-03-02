/* Written by Jamy Spencer 23 Feb 2017 */
#ifndef SLAVEOBJ_H
#define SLAVEOBJ_H

#define MAX_USERS 15

#ifndef BILLION
#define BILLION 1000000000
#endif
#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN 12
#endif


struct info{
	pid_t process_id;
	char* msg;

}; 


struct list{
	struct info item;
	struct list* next;
	struct list* prev;
};


typedef struct queue_msg{
	long int mtype;
	pid_t pid;
	char mtext[1];
} msg_t;

struct timespec* shrMemMakeAttach(int* shmid);
int lockMsgMakeAttach(void);

#endif
