CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra
LDFLAGS = 
HEADERS = $(shell ${CXX} ${CXXFLAGS} -MM *.cpp | sed -E 's/.+\.[^h]pp//g')

all: test
	./$<

%: %.cpp

%.o: %.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c -o $@ $<


%: %.o ${OBJS}
	${CXX} ${LDFLAGS} -o $@ $^
