OBJS = TcpSocket.o ConnectionManager.o
LIBS = ../json/libjson.a
CPPFLAGS = -I../

all: test.ok libnetwork.a

libnetwork.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
