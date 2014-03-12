PKGCONFIG = pkg-config

OBJS_GUI = user/GraphicUserManager.o stadium/GraphicStadiumManager.o match/GraphicMatchManager.o market/GraphicMarketManager.o fgame/GUIFGameManager.o team/GraphicTeamManager.o championship/GraphicChampionshipManager.o

OBJS_CLI = cli/CLI.o cli/Menu.o user/CLIUserManager.o stadium/CLIStadiumManager.o team/CLITeamManager.o market/CLIMarketManager.o match/CLIMatchManager.o fgame/CLIFGameManager.o championship/CLIChampionshipManager.o

OBJS_CONTROLLER = user/UserManager.o stadium/StadiumManager.o team/TeamManager.o market/MarketManager.o match/MatchManager.o fgame/FGameManager.o ClientManager.o championship/ChampionshipManager.o

OBJS = ${OBJS_CONTROLLER} ${OBJS_GUI} ${OBJS_CLI}

LIBS = ../common/json/libjson.a ../common/network/libnetwork.a ../common/model/libmodel.a graphics/libgraphics.a

CPPFLAGS = -I../common -I.

ifeq ($(shell uname -s),Darwin)
        CPPFLAGS += -I/Library/Frameworks/SFML.framework/Headers/
else
        CPPFLAGS += $(shell ${PKGCONFIG} --cflags sfml-all)
endif

.PHONY: graphics/libgraphics.a admin
 
all: client.app graphicClient.app admin

client.app: main.o Client.o ${OBJS_CONTROLLER} ${OBJS_CLI} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

graphicClient.app: GraphicMain.o GraphicClient.o ${OBJS_CONTROLLER} ${OBJS_GUI} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^
 
graphics/libgraphics.a:
	+make -C graphics/
 
include ../Makefile.includes
ifeq ($(shell uname -s),Darwin)
	LDFLAGS += -framework sfml-system -framework sfml-graphics -framework sfml-window
else
	LDFLAGS += $(shell ${PKGCONFIG} --libs sfml-all)
endif
