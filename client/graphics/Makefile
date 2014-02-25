OBJS = 
PKGCONFIG = PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ pkg-config
CPPFLAGS = -I/Library/Frameworks/SFML.framework/Headers/ -I../common

playground.app: playground.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

include ../../Makefile.includes

LDFLAGS += -framework sfml-system -framework sfml-graphics -framework sfml-window