/* Written by Jamy Spencer 7 Feb 2017 */
#ifndef FORKERLIB_H
#define FORKERLIB_H

#include <time.h>


data_t CreateLogMsg(char* msg, char* prg_name, char* spec_num);
int addmsg(data_t data);
void clearlog(void);
char* getlog(void);
int SaveLog(char* log_file_name);
int MakeSlaves(int num_slaves);

#endif
