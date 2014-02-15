OBJS = Installation.o
CPPFLAGS = -I..
LIBS = ../json/libjson.a

all: test.ok libmodel.a

libmodel.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
