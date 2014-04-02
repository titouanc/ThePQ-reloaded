include SFML.includes

OBJS_GUI = user/GraphicUserManager.o stadium/GraphicStadiumManager.o match/GraphicMatchManager.o market/GraphicMarketView.o fgame/GraphicFGameView.o team/GraphicTeamManager.o championship/GraphicChampionshipView.o

OBJS_CLI = cli/CLI.o cli/Menu.o user/CLIUserManager.o stadium/CLIStadiumManager.o team/CLITeamManager.o market/CLIMarketView.o match/CLIMatchManager.o fgame/CLIFGameView.o championship/CLIChampionshipView.o

OBJS_CONTROLLER = user/UserManager.o stadium/StadiumManager.o team/TeamManager.o market/MarketController.o match/MatchManager.o fgame/FGameController.o ClientController.o championship/ChampionshipController.o

OBJS = ${OBJS_CONTROLLER} ${OBJS_GUI} ${OBJS_CLI}

LIBS = ../common/json/libjson.a ../common/network/libnetwork.a ../common/model/libmodel.a graphics/libgraphics.a

CPPFLAGS = -I../common -I. ${SFML_CPPFLAGS}

.PHONY: graphics/libgraphics.a admin/adminClient.app
 
all: client.app graphicClient.app admin/adminClient.app

admin/adminClient.app:
	+make -C admin/

client.app: main.o Client.o ${OBJS_CONTROLLER} ${OBJS_CLI} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

graphicClient.app: GraphicMain.o GraphicClient.o ${OBJS_CONTROLLER} ${OBJS_GUI} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^
 
graphics/libgraphics.a:
	+make -C graphics/
 
include ../Makefile.includes

LDFLAGS += ${SFML_LDFLAGS}
