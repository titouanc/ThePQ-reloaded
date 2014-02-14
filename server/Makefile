OBJS= Server.o User.o
LIBNETWORK=../common/network/libnetwork.a
LIBJSON=../common/json/libjson.a
LIBPITCH=../common/pitch/libpitch.a
LIBS = ${LIBPITCH} ${LIBJSON} ${LIBNETWORK}

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -pthread -I../common/
LDFLAGS = -lm -lpthread

all: server.app test.app
	./test.app

server.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

test.app: test.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

clean:
	rm -f *.o

mrproper: clean
	rm -f server.app

.PHONY: clean all mrproper
.PRECIOUS: ${OBJS}
