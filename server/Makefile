OBJS= Server.o User.o MatchManager.o RandomNameGenerator.o PlayerMarket.o Sale.o
LIBS = ../common/network/libnetwork.a ../common/json/libjson.a ../common/model/libmodel.a

CPPFLAGS = -I../common/

all: test.ok server.app

server.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

include ../Makefile.includes
