PKGCONFIG = PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ pkg-config
OBJS = UIMatch.o
LIBS = ../../common/model/libmodel.a ../../common/json/libjson.a
ifeq ($(shell uname -s),Darwin)
	CPPFLAGS = -I/Library/Frameworks/SFML.framework/Headers/ -I../../common
else
	CPPFLAGS = $(shell ${PKGCONFIG} --cflags sfml-all) -I../../common/
endif

all: playground.app
	LD_LIBRARY_PATH=/usr/local/lib/:${LD_LIBRARY_PATH} ./$<

include ../../Makefile.includes

ifeq ($(shell uname -s),Darwin)
	LDFLAGS += -framework sfml-system -framework sfml-graphics -framework sfml-window
else
	LDFLAGS += $(shell ${PKGCONFIG} --libs sfml-all)
endif
