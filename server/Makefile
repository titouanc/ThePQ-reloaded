OBJS= Server.o MatchManager.o PlayerMarket.o AdminManager.o
LIBS = ../common/network/libnetwork.a ../common/json/libjson.a ../common/model/libmodel.a

CPPFLAGS = -I../common/

all: test.ok server.app

server.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

.PHONY: rmusers
rmusers:
	rm -r data/users/*

include ../Makefile.includes
