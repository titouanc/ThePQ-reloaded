OBJS = ClientConnectionManager.o BaseConnectionManager.o ConnectionManager.o SubConnectionManager.o
LIBS = ../json/libjson.a
CPPFLAGS = -I../

all: libnetwork.a

libnetwork.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
