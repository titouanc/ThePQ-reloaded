CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../lighttest
LDFLAGS = -lm
HEADERS = $(shell ${CXX} ${CXXFLAGS} -MM *.cpp | sed -E 's/.+\.[^h]pp//g')
OBJS = parser.o number.o string.o list.o dict.o load.o
LIB = libjson.a

all: ${LIB} test.ok

test.ok: test
	./$< && touch $@

%: %.cpp

%.o: %.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c -o $@ $<

%.a: ${OBJS}
	ar r $@ $^

%: %.o ${LIB}
	${CXX} ${LDFLAGS} -o $@ $^

.PRECIOUS: ${OBJS}
.PHONY: clean mrproper
clean:
	rm -f *.o *.ok

mrproper: clean
	rm -f test jsondump ${LIB}
