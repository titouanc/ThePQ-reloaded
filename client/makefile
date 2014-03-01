OBJS = Menu.o ClientMatchManager.o Client.o UserManager.o StadiumManager.o TeamManager.o MarketManager.o
LIBS = ../common/json/libjson.a ../common/network/libnetwork.a ../common/model/libmodel.a graphics/libgraphics.a
CPPFLAGS = -I../common

.PHONY: graphics/libgraphics.a

all: client.app graphicClient.app

client.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

graphicClient.app: GraphicMain.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

graphics/libgraphics.a:
	make -C graphics/

include ../Makefile.includes
