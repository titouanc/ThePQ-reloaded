CPPFLAGS = -I../lighttest
OBJS = parser.o string.o list.o dict.o value.o number.o load.o

all: libjson.a test.ok

libjson.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
