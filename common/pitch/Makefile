CPPFLAGS = -I../
OBJS = 
LIBS = ../json/libjson.a ../model/libmodel.a

all: libpitch.a test.ok moveableTest.ok

libpitch.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
