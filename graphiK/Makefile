PKGCONFIG = PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ pkg-config

CPPFLAGS = $(shell ${PKGCONFIG} --cflags sfml-all) -I../common/
LIBS = ../common/model/libmodel.a ../common/json/libjson.a

all: testsfml.app
	LD_LIBRARY_PATH=/usr/local/lib/ ./$<

include ../Makefile.includes

LDFLAGS += $(shell ${PKGCONFIG} --libs sfml-all)
