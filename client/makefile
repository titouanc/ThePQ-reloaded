PKGCONFIG = pkg-config
OBJS = Menu.o ClientMatchManager.o Client.o UserManager.o StadiumManager.o TeamManager.o MarketManager.o GraphicUserManager.o GraphicStadiumManager.o NotificationManager.o
LIBS = ../common/json/libjson.a ../common/network/libnetwork.a ../common/model/libmodel.a graphics/libgraphics.a
CPPFLAGS = -I../common
 
ifeq ($(shell uname -s),Darwin)
        CPPFLAGS += -I/Library/Frameworks/SFML.framework/Headers/
else
        CPPFLAGS += $(shell ${PKGCONFIG} --cflags sfml-all)
endif
 
.PHONY: graphics/libgraphics.a
 
all: client.app graphicClient.app admin
 
admin:
	make -C admin
 
client.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^
 
graphicClient.app: GraphicMain.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^
 
graphics/libgraphics.a:
	+make -C graphics/
 
include ../Makefile.includes
ifeq ($(shell uname -s),Darwin)
	LDFLAGS += -framework sfml-system -framework sfml-graphics -framework sfml-window
else
	LDFLAGS += $(shell ${PKGCONFIG} --libs sfml-all)
endif
