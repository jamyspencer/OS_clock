#ifndef SLAVEOBJ
#define SLAVEOBJ

struct info{
	pid_t process_id;
}; 


struct list{
	struct info item;
	struct list* next;
};


#define SLV_INFO struct info
#define SLV_LIST struct list

#endif
