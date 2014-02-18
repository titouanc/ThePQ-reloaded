OBJS = ConnectionManager.o
LIBS = ../json/libjson.a
CPPFLAGS = -I../

all: libnetwork.a

libnetwork.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
