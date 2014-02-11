CXX = g++
CPPFLAGS = -I../common/json/
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -pthread
LDFLAGS = -lm -lpthread
HEADERS = $(shell ${CXX} ${CPPFLAGS} -MM *.cpp | sed -E 's/.+\.[^h]pp//g')
OBJS = connectionmanager.o

all: test.ok

test.ok: test
	./$< && touch $@

%: %.cpp

%.o: %.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} ${INCLUDES} -c -o $@ $<

%: %.o ${OBJS} ../common/json/libjson.a
	${CXX} ${LDFLAGS} -o $@ $^

.PRECIOUS: ${OBJS}
.PHONY: clean mrproper
clean:
	rm -f *.o *.ok

mrproper: clean
	rm -f test
