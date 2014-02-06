OBJS = $(wildcard *.o)
MAINNAME = unitTests
LIBNETWORK = ../build/libnetwork.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra `pkg-config --cflags cppunit` -I../src/
LDFLAGS = `pkg-config --libs cppunit`

all: ${MAINNAME}
	./$<

${MAINNAME}: ${OBJS} ${LIBNETWORK}
	${CXX} ${LDFLAGS} -o $@ $^

${LIBNETWORK}: ../src/*
	cd .. && make lib

%.o: %.cpp *.h
	${CXX} ${CXXFLAGS} -c -o $@ $<

clean:
	rm -f ${OBJS}

mrproper: clean
	rm -f ${MAINNAME}

.PHONY: clean all mrproper
