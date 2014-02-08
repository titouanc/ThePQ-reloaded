OBJ=Listener.o Socket.o TcpSocket.o
SERV = serv.app
CLI = cli.app
LIBNETWORK=libnetwork.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../json
LDFLAGS = -L../json -ljson

all: ${SERV} ${CLI}

${SERV}: mainserver.o ${LIBNETWORK}
	${CXX} ${LDFLAGS} -o $@ $^
	
${CLI}: mainclient.o ${LIBNETWORK}
	${CXX} ${LDFLAGS} -o $@ $^

${LIBNETWORK}: ${OBJ}
	ar r libnetwork.a ${OBJ}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

clean:
	rm -f *.o

mrproper: clean
	rm -f ${SERV}
	rm -f ${CLI}

.PHONY: clean all mrproper
