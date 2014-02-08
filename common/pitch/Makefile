CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../json/ -I../lighttest
LDFLAGS = -lm -L../json -ljson
HEADERS = $(shell ${CXX} ${CXXFLAGS} -MM *.cpp | sed -E 's/.+\.[^h]pp//g')
OBJS = position.o displacement.o

all: test.ok

test.ok: test
	./$< && touch $@

%: %.cpp

%.o: %.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c -o $@ $<

%: %.o ${OBJS}
	${CXX} ${LDFLAGS} -o $@ $^

.PRECIOUS: ${OBJS}
.PHONY: clean mrproper
clean:
	rm -f *.o *.ok

mrproper: clean
	rm -f test
