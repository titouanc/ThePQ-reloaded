CXX = g++
INCLUDES = -I../../common/lighttest/ -I../../common/json/
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra
LDFLAGS = -lm ../../common/json/libjson.a
HEADERS = $(shell ${CXX} ${CXXFLAGS} -MM *.cpp | sed -E 's/.+\.[^h]pp//g')
OBJS = connectionmanager.o

all: test.ok

test.ok: test
	./$< && touch $@

%: %.cpp

%.o: %.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} ${INCLUDES} -c -o $@ $<

%: %.o ${OBJS}
	${CXX} ${LDFLAGS} -o $@ $^

.PRECIOUS: ${OBJS}
.PHONY: clean mrproper
clean:
	rm -f *.o *.ok

mrproper: clean
	rm -f test
