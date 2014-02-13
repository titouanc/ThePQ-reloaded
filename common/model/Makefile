OBJ=Installation.o
CXX=g++
CFLAGS=-std=c++11 -pedantic -Wall -Wextra -I../json -I../lighttest
LDFLAGS= ../json/libjson.a

all: test.app
	./test.app
	
test.app: InstallationTest.o ${OBJ}
	${CXX} ${LDFLAGS} -o $@ $^
	
%.o: %.cpp
	${CXX} ${CFLAGS} -c -o $@ $^

clean:
	rm -f ${OBJ}
	
mrproper:
	rm -f test.app
