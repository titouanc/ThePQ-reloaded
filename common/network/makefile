OBJ= Socket.o TcpSocket.o Message.o
SERV = serv.app
CLI = cli.app
LIBNETWORK=libnetwork.a
LIBJSON=../json/libjson.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../json
LDFLAGS = 

all: ${SERV} ${CLI}

${SERV}: mainserver.o ${LIBNETWORK} ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^
	
${CLI}: mainclient.o ${LIBNETWORK} ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^

${LIBNETWORK}: ${OBJ}
	ar r libnetwork.a ${OBJ}
	
${LIBJSON}:
	make -C ../json

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

clean:
	rm -f *.o

mrproper: clean
	rm -f ${SERV}
	rm -f ${CLI}
	rm -f ${LIBNETWORK}

.PHONY: clean all mrproper
