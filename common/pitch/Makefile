CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -I../json/ -I../lighttest
LDFLAGS = -lm
HEADERS = $(shell ${CXX} ${CXXFLAGS} -MM *.cpp | sed -E 's/.+\.[^h]pp//g')
OBJS = position.o displacement.o
TESTS = test.ok moveableTest.ok

all: ${TESTS}

%.ok: %.app
	./$< && touch $@

%.o: %.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c -o $@ $<

%.app: %.o ${OBJS} ../json/libjson.a
	${CXX} ${LDFLAGS} -o $@ $^

.PRECIOUS: ${OBJS} $(TESTS:.ok=.app) $(TESTS:.ok=.o)
.PHONY: clean mrproper
clean:
	rm -f *.o *.ok

mrproper: clean
	rm -f *.app
