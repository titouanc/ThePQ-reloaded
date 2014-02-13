OBJ= Server.o User.o
LIBNETWORK=../common/network/libnetwork.a
LIBJSON=../common/json/libjson.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -pthread -I../common/json -I../common/lighttest -I../common/network
LDFLAGS = -lm -lpthread

all: server.app test.app
	./test.app

server.app: main.o ${OBJ} ${LIBNETWORK} ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^

test.app: test.o User.o ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $^

${LIBNETWORK}:
	cd ../common/network && make
	
${LIBJSON}:
	cd ../common/json && make

clean:
	rm -f *.o

mrproper: clean
	rm -f server.app

.PHONY: clean all mrproper
.PRECIOUS: ${OBJS}
