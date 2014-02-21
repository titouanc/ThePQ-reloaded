OBJS = Position.o Displacement.o Pitch.o Player.o Installation.o Sale.o
CPPFLAGS = -I..
LIBS = ../json/libjson.a

all: test.ok serializationTest.ok moveableTest.ok test_pitch_position_displacement.ok libmodel.a

libmodel.a: ${OBJS}
	ar r $@ $^

include ../../Makefile.includes
