OBJS = Position.o Displacement.o Pitch.o Player.o  Installation.o 
CPPFLAGS = -I..
LIBS = ../json/libjson.a

all: test.ok serializationTest.ok libmodel.a

libmodel.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
