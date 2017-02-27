/* Written by Jamy Spencer 23 Feb 2017 */
#ifndef FORKERLIB_H
#define FORKERLIB_H

#include "slaveobj.h"

SLV_LIST* MakeChildren(SLV_LIST* head_ptr, int* child_count, int* total_spawned, int num_children);
void KillSlaves(SLV_LIST* hd_ptr);
char* MakeMsg(struct timespec* clock, char* x_time);
int SaveLog(char* log_file_name, char* msg);
void clock_tick(struct timespec *clock, int increment);
struct list *returnTail(struct list *head_ptr);
struct list *addNode(struct list *head_ptr, pid_t pid);
struct list* destroyNode(struct list *head_ptr, pid_t pid);
void destroyHead(struct list *head_ptr);
struct list* findNodeByPid(struct list *head_ptr, pid_t pid);

#endif
