#ifndef DEFINE_UTILS_HEADER
#define DEFINE_UTILS_HEADER

#include <cassert>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

/* Sleep for given number of seconds */
static inline void minisleep(double secs)
{
	assert(secs >= 0);
	int seconds = secs;
	int micros  = secs*1000000 - seconds;

	struct timeval interval = {seconds, micros};
	select(1, NULL, NULL, NULL, &interval);
}

#endif