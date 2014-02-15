OBJS= Server.o User.o MatchManager.o
LIBS = ../common/network/libnetwork.a ../common/json/libjson.a ../common/pitch/libpitch.a
CPPFLAGS = -I../common/

all: test.ok server.app

server.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

include ../Makefile.includes
