/* Written by Jamy Spencer 23 Feb 2017 */
#ifndef SLAVEOBJ_H
#define SLAVEOBJ_H

#define MAX_USERS 15

#ifndef BILLION
#define BILLION 1000000000
#endif



struct info{
	pid_t process_id;
	int queue_num;
}; 


struct list{
	struct info item;
	struct list* next;
	struct list* prev;
};


typedef struct queue_msg{
	long mtype;
	char mtext[15];
} msg_t;


struct timespec* shrMemMakeAttach(int* shmid);
int lockMsgMakeAttach(void);
struct list *returnTail(struct list *head_ptr);
void addNode(struct list *head_ptr, pid_t pid);
struct list* destroyNode(struct list *head_ptr, pid_t pid);
struct list* destroyHead(struct list *head_ptr);


#define SLV_INFO struct info
#define SLV_LIST struct list


#endif
