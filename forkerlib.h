/* Written by Jamy Spencer 7 Feb 2017 */
#ifndef FORKERLIB_H
#define FORKERLIB_H

#include "slaveobj.h"

SLV_LIST* MakeSlaves(int num_slaves, SLV_LIST* hd_ptr);
void KillSlaves(SLV_LIST* hd_ptr);

#endif
