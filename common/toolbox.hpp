#ifndef DEFINE_UTILS_HEADER
#define DEFINE_UTILS_HEADER

#include <cstring>
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


#define STRIP_JUNK "\b\r\n\t "
static inline char *lstrip(char *str, const char *junk=STRIP_JUNK)
{
	assert(str != NULL);
	while (*str && strchr(STRIP_JUNK, *str))
		str++;
	return str;
}

static inline char *rstrip(char *str, const char *junk=STRIP_JUNK)
{
	assert(str != NULL);
	char *endptr = str + strlen(str) - 1;
	while (endptr >= str && strchr(STRIP_JUNK, *endptr)){
		*endptr = '\0';
		endptr --;
	}
	return str;
}

static inline char *strip(char *str, const char *junk=STRIP_JUNK)
{
	return rstrip(lstrip(str));
}

#endif