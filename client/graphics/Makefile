include ../SFML.includes

OBJS = Layer.o MainController.o GraphicManager.o UIMatch.o loadPath.o
CPPFLAGS = -I../../common

LIBS = ../../common/model/libmodel.a ../../common/json/libjson.a
CPPFLAGS += ${SFML_CPPFLAGS}

#all: libgraphics.a testbox.app
all: libgraphics.a
libgraphics.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes

LDFLAGS += ${SFML_LDFLAGS}
