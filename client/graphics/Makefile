PKGCONFIG = pkg-config
OBJS = Layer.o MainController.o GraphicManager.o UIMatch.o loadPath.o
CPPFLAGS = -I../../common

LIBS = ../../common/model/libmodel.a ../../common/json/libjson.a
ifeq ($(shell uname -s),Darwin)
	CPPFLAGS += -I/Library/Frameworks/SFML.framework/Headers/
else
	CPPFLAGS += $(shell ${PKGCONFIG} --cflags sfml-all)
endif

all: playground.app libgraphics.a

libgraphics.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes

ifeq ($(shell uname -s),Darwin)
	LDFLAGS += -framework sfml-system -framework sfml-graphics -framework sfml-window
else
	LDFLAGS += $(shell ${PKGCONFIG} --libs sfml-all)
endif
