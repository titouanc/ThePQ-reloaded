#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#define ADDR "127.0.0.1"

int main(int argc, const char **argv)
{
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
    addr.sin_port = htons(32123);
    if (inet_aton(ADDR, &(addr.sin_addr)) == 0){
        fprintf(stderr, "Unable to parse address %s: %s", ADDR, strerror(errno));
        return -1;
    }

    if (connect(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) != 0){
    	fprintf(stderr, "Unable to connect to %s: %s\n", ADDR, strerror(errno));
    	return -1;
    }

    printf("Connected to %s...\n", ADDR);

    const char *str = "{\"key\": \"value\"}";
    int r = send(sock, str, strlen(str)+1, 0);
  	printf("Done !\n");

    close(sock);
    return 0;
}
