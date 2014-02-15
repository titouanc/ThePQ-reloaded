CPPFLAGS = -I../
OBJS = Position.o Displacement.o Pitch.o
LIBS = ../json/libjson.a

all: libpitch.a test.ok moveableTest.ok

libpitch.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
