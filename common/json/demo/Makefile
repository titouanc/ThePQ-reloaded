CXX = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra
INCLUDES = -I../
LDFLAGS = -lm

DEMOS = create

all: ${DEMOS}
	for exe in $^; do echo ===== $$exe ===== && ./$$exe; done

clean:
	rm -f *.o ${DEMOS}

../libjson.a:
	make -C ..

%: %.cpp ../libjson.a
	${CXX} ${CXXFLAGS} ${INCLUDES} ${LDFLAGS} -o $@ $^
