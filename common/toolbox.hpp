#ifndef DEFINE_UTILS_HEADER
#define DEFINE_UTILS_HEADER

#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string>

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
	while (*str && strchr(junk, *str))
		str++;
	return str;
}

static inline char *rstrip(char *str, const char *junk=STRIP_JUNK)
{
	assert(str != NULL);
	char *endptr = str + strlen(str) - 1;
	while (endptr >= str && strchr(junk, *endptr)){
		*endptr = '\0';
		endptr --;
	}
	return str;
}

static inline char *strip(char *str, const char *junk=STRIP_JUNK)
{
	return rstrip(lstrip(str, junk), junk);
}

static inline std::string strip(std::string str, const char *junk=STRIP_JUNK)
{
	if (str.length() == 0) return str;
	unsigned int begin=0, end=str.length()-1;
	for (; begin<str.length() && strchr(junk, str[begin]); ++begin);
	for (; end > begin && strchr(junk, str[end]); --end);
	return std::string(str, begin, end-begin+1);
}

#endif