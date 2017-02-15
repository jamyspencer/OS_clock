/* Written by Jamy Spencer 7 Feb 2017 */
#ifndef FORKERLIB_H
#define FORKERLIB_H

#include "slaveobj.h"

SLV_LIST* MakeSlave(SLV_LIST* hd_ptr);
void KillSlaves(SLV_LIST* hd_ptr);
int AllocateSharedMemory(int*);

#endif
