OBJS = Menu.o Message.o CLI.o
LIBS = ../common/json/libjson.a ../common/network/libnetwork.a ../common/model/libmodel.a
CPPFLAGS = -I../common

client.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

include ../Makefile.includes
