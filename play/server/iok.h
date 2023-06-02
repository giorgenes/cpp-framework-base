#ifndef __iok_h
#define __iok_h

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <sys/time.h>

int trrmgr_iok_accept(int, int, int, int*, int*);
int trrmgr_iok_recv(int, void*, size_t, struct timeval*, int*, int);
int trrmgr_iok_send(int, const void*, size_t, struct timeval*, int*);
int trrmgr_iok_connect(const char*, int, struct timeval*, int*);
int trrmgr_iok_setfail(int p_fail);

#endif
