OBJ = Installation.o
CXX = g++
CFLAGS = -std=c++11 -pedantic -Wall -Wextra -I..
LDFLAGS = ../json/libjson.a

all: InstallationTest.ok

%.ok: %.app
	./$< && touch $@
	
%.app: %.o ${OBJ}
	${CXX} ${LDFLAGS} -o $@ $^
	
%.o: %.cpp
	${CXX} ${CFLAGS} -c -o $@ $<

.PHONY: clean mrproper all
.PRECIOUS: ${OBJ}

clean:
	rm -f ${OBJ} *.ok
	
mrproper: clean
	rm -f *.app
