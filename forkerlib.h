/* Written by Jamy Spencer 23 Feb 2017 */
#ifndef FORKERLIB_H
#define FORKERLIB_H

#include "slaveobj.h"

struct list *MakeChildren(struct list *head_ptr, int* child_count, int* total_spawned, int num_children);
void KillSlaves(struct list *hd_ptr, char* file_name);
char* MakeMsg(struct timespec* clock, char* x_time, pid_t pid);
int SaveLog(char* log_file_name, char* msg, pid_t pid);
void clock_tick(struct timespec *clock, int increment);
void appendMsg(struct list* head_ptr, struct timespec *clock, pid_t pid, char* done_time);
struct list *returnTail(struct list *head_ptr);
struct list *addNode(struct list *head_ptr, pid_t pid);
struct list* destroyNode(struct list *head_ptr, pid_t pid, char* file_name);

struct list* findNodeByPid(struct list *head_ptr, pid_t pid);

#endif
