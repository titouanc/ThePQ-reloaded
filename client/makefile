PKGCONFIG = pkg-config
OBJS = cli/Menu.o Client.o GraphicClient.o user/UserManager.o stadium/StadiumManager.o team/TeamManager.o market/MarketManager.o user/GraphicUserManager.o stadium/GraphicStadiumManager.o match/GraphicMatchManager.o match/MatchManager.o user/CLIUserManager.o stadium/CLIStadiumManager.o team/CLITeamManager.o market/CLIMarketManager.o match/CLIMatchManager.o ClientManager.o cli/CLI.o fgame/FGameManager.o fgame/CLIFGameManager.o
LIBS = ../common/json/libjson.a ../common/network/libnetwork.a ../common/model/libmodel.a graphics/libgraphics.a
CPPFLAGS = -I../common -I.

ifeq ($(shell uname -s),Darwin)
        CPPFLAGS += -I/Library/Frameworks/SFML.framework/Headers/
else
        CPPFLAGS += $(shell ${PKGCONFIG} --cflags sfml-all)
endif
 
.PHONY: graphics/libgraphics.a admin
 
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
