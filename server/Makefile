OBJS= Server.o MatchManager.o PlayerMarket.o
LIBS = ../common/network/libnetwork.a ../common/json/libjson.a ../common/model/libmodel.a

CPPFLAGS = -I../common/

all: test.ok server.app

server.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

.PHONY: rmusers
rmusers:
	rm -r data/users/*
	rm data/global/userNames.json
	rm data/global/teamNames.json

include ../Makefile.includes
