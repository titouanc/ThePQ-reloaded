OBJ= Server.o
LIBNETWORK=../common/network/libnetwork.a
LIBJSON=../common/json/libjson.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -pthread -I../common/json
LDFLAGS = -lm -lpthread

all: server.app

server.app: main.o ${OBJ} ${LIBNETWORK} ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

clean:
	rm -f *.o

mrproper: clean
	rm -f server.app

.PHONY: clean all mrproper
.PRECIOUS: ${OBJS}