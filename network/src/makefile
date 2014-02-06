OBJS = $(wildcard *.o)
LIBNETWORK = ../build/libnetwork.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra
LDFLAGS =

all: ${LIBNETWORK}

%.o: %.cpp *.h
	${CXX} ${CXXFLAGS} -c -o $@ $<

${LIBNETWORK}: ${OBJS}
	ar r $@ $^

clean:
	rm -f ${OBJS}

mrproper: clean
	rm -f ${LIBNETWORK}

.PHONY: clean all mrproper
