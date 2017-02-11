#ifndef SLAVELIB_H
#define SLAVELIB_H


char* MakeMsg(int shared_num);
void WriteMsg(void);
void CriticalSection(char* save_file);
int SaveLog(char* log_file_name);

#endif
