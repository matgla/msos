#ifndef _MACHTIME_H_
#define _MACHTIME_H_

#define _CLOCKS_PER_SEC_ 1000000

#include <sys/_timespec.h>
extern "C"
{
int nanosleep(const struct timespec*, struct timespec*);
}

#endif
