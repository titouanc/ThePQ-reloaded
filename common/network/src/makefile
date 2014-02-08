SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
LIBNETWORK=../build/libnetwork.a
LIBJSON=libjson.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../../json
LDFLAGS = -L../../json -ljson

all: ${LIBNETWORK}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

${LIBNETWORK}: ../build ${OBJ}
	ar r $@ ${OBJ}

../build:
	mkdir $@

clean:
	rm -f ${OBJ}

mrproper: clean
	rm -rf ../build

.PHONY: clean all mrproper
