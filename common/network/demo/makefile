SERV = serv
CLI = cli
LIBNETWORK = ../build/libnetwork.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../src/
LDFLAGS =

all: ${SERV} ${CLI}

${SERV}: mainserver.o ${LIBNETWORK} 
	${CXX} ${LDFLAGS} -o $@ $^
	
${CLI}: mainclient.o ${LIBNETWORK} 
	${CXX} ${LDFLAGS} -o $@ $^

${LIBNETWORK}: 
	cd ../src && make all

mainserver.o:
	${CXX} ${CXXFLAGS} -c -o $@ mainserver.cpp
	
mainclient.o:
	${CXX} ${CXXFLAGS} -c -o $@ mainclient.cpp

clean:
	rm -f mainserver.o
	rm -f mainclient.o

mrproper: clean
	rm -f ${MAIN}

.PHONY: clean all mrproper
