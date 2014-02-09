OBJ= main.o Client.o Connexion.o Messages.o
MAIN=client.app
CXX=g++
CFLAGS=-std=c++11 -pedantic -Wall -Wextra -I../common/network -I../common/json
LDFLAGS= 
LIBNETWORK=../common/network/libnetwork.a
LIBJSON=../common/json/libjson.a

all: ${MAIN}

${MAIN}: ${OBJ} ${LIBNETWORK} ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^

%.o: %.cpp
	${CXX} ${CFLAGS} -c -o $@ $^

${LIBNETWORK}:
	cd ../common/network && make
	
${LIBJSON}:
	cd ../common/json && make

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ${MAIN}
