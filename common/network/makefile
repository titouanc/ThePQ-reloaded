OBJ= test.o TcpSocket.o ConnectionManager.o
LIBNETWORK=libnetwork.a
LIBJSON=../json/libjson.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -pthread -I../
LDFLAGS = -lm -lpthread

all: test.app

test.app: ${OBJ} ${LIBNETWORK} ${LIBJSON}
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
	rm -f test.app
	rm -f ${LIBNETWORK}

.PHONY: clean all mrproper
.PRECIOUS: ${OBJS}
