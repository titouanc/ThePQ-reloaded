CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra
LDFLAGS = -lm
HEADERS = $(shell ${CXX} ${CXXFLAGS} -MM *.cpp | sed -E 's/.+\.[^h]pp//g')
OBJS = parser.o number.o string.o list.o dict.o

all: test
	./$<

%: %.cpp

%.o: %.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c -o $@ $<


%: %.o ${OBJS}
	${CXX} ${LDFLAGS} -o $@ $^

.PRECIOUS: ${OBJS}
.PHONY: clean mrproper
clean:
	rm -f *.o *.dep

mrproper: clean
	rm -f test