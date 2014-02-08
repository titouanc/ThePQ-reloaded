SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
LIBNETWORK=../build/libnetwork.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra
LDFLAGS =

all: ${LIBNETWORK}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

${LIBNETWORK}: ${OBJ}
	ar r $@ $^

clean:
	rm -f ${OBJ}

mrproper: clean
	rm -f ${LIBNETWORK}

.PHONY: clean all mrproper
