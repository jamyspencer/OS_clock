/* Written by Jamy Spencer 23 Feb 2017 */
#ifndef FORKERLIB_H
#define FORKERLIB_H

#include "slaveobj.h"

SLV_LIST* MakeChildren(SLV_LIST* head_ptr, int* child_count, int* total_spawned, int num_children);
void KillSlaves(SLV_LIST* hd_ptr);
char* MakeMsg(int* shr_tot);
int SaveLog(char* log_file_name, char* msg);
void clock_tick(int* clock, int increment);

#endif
