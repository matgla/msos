#ifndef _MACHTIME_H_
#define _MACHTIME_H_

#define _CLOCKS_PER_SEC_ 1000000

#include <sys/_timespec.h>

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus
int nanosleep(const struct timespec*, struct timespec*);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // _MACHTIME_H