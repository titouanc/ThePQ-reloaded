/*
 * jsondump.cpp: Display the content of a JSON file
 * 
 * History:
 *    + 2014-02-07: First release (Titou)
 */

#include "json.h"
#include <iostream>

extern "C" {
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <string.h>
}

using namespace std;

static inline void indent(size_t n, const char *indenter="  ")
{
	while (n>0){
		cout << indenter; 
		n--;
	}
}

/* Recursive walk in JSON data */
static void exploreJSON(const JSON::Value *elem, size_t depth=0)
{
	if (ISLIST(elem)){
		cout << "[" << endl;
		for (size_t i=0; i<LIST(elem).len(); i++){
			indent(depth+1);
			exploreJSON(LIST(elem)[i], depth+1);
		}
		indent(depth);
		cout << "]" << endl;
	}

	else if (ISDICT(elem)){
		cout << "{" << endl;
		JSON::Dict::iterator it;
		for (it = DICT(elem).begin(); it != DICT(elem).end(); it++){
			indent(depth+1);
			cout << it->first << " : ";
			exploreJSON(it->second, depth+1);
		}
		cout << "}" << endl;
	}

	else if (ISINT(elem))
		cout << INT(elem).value() << endl;

	else if (ISFLOAT(elem))
		cout << FLOAT(elem).value() << endl;
	
	else if (ISSTR(elem))
		cout << elem->dumps() << endl;
}

/* 16kB */
#define BUFSIZE 0x10000

int main(int argc, const char **argv)
{
	char buffer[BUFSIZE] = {'\0'};

	if (argc == 1){
		cerr << "Usage: " << argv[0] << " file.json [ file.json ... ]" << endl;
		return EXIT_SUCCESS;
	}

	for (int i=1; i<argc; i++){
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0){
			cerr << "Cannot open " << argv[i] << ": " << strerror(errno) << endl;
			continue;
		}

		int r = read(fd, buffer, BUFSIZE-1);
		
		if (r < 0){
			cerr << "Cannot read " << argv[i] << ": " << strerror(errno) << endl;
		} else {
			buffer[r] = '\0';
			JSON::Value *json = NULL;
			try {
				json = JSON::parse(buffer);
				cout << argv[i] << endl;
				exploreJSON(json);
				delete json;
			} catch (JSON::ParseError & err) {
				cerr << "Cannot parse " << argv[i] << ": " << err.what() << endl;
			}
		}

		close(fd);
		indent(80, "-");
		cout << endl;
	}

	return EXIT_SUCCESS;
}
