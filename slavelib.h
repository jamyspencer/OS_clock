#ifndef SLAVELIB_H
#define SLAVELIB_H

struct slave_info{
	pid_t process_id;
};

void CriticalSection(void);
void WriteMsg(void);

#endif
