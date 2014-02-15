OBJ=Installation.o InstallationTest.o
CXX=g++
CFLAGS=-std=c++11 -pedantic -Wall -Wextra -I..
LDFLAGS= ../json/libjson.a

all: test.app
	./test.app
	
test.app: ${OBJ}
	${CXX} ${LDFLAGS} -o $@ $^
	
%.o: %.cpp
	${CXX} ${CFLAGS} -c -o $@ $^

clean:
	rm -f ${OBJ}
	
mrproper: clean
	rm -f test.app
