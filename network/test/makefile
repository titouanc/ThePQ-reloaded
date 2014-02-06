SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
MAIN = unitTests.app
LIBNETWORK = ../build/libnetwork.a

CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra `pkg-config --cflags cppunit` -I../src/
LDFLAGS = `pkg-config --libs cppunit`

all: ${MAIN}
	./$<

${MAIN}: ${OBJ} ${LIBNETWORK}
	${CXX} ${LDFLAGS} -o $@ $^

${LIBNETWORK}: 
	cd ../src && make all

$(OBJ): $(SRC)
	${CXX} ${CXXFLAGS} -c -o $@ $<

clean:
	rm -f ${OBJ}

mrproper: clean
	rm -f ${MAIN}

.PHONY: clean all mrproper
