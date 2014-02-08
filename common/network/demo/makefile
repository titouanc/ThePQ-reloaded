SERV = serv.app
CLI = cli.app
LIBNETWORK=../build/libnetwork.a
LIBJSON=../../json/libjson.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../src/ -I../../json
LDFLAGS =

all: ${SERV} ${CLI}

${SERV}: mainserver.o ${LIBNETWORK} ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^
	
${CLI}: mainclient.o ${LIBNETWORK}  ${LIBJSON}
	${CXX} ${LDFLAGS} -o $@ $^

${LIBNETWORK}:
	cd ../src && make all
	
${LIBJSON}:
	cd ../../json && make all

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
