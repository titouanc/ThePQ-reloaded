all: adminClient.app
OBJS = AdminClient.o ../cli/Menu.o ../cli/CLI.o
LIBS = ../../common/json/libjson.a ../../common/network/libnetwork.a ../../common/model/libmodel.a
CPPFLAGS = -I../../common

adminClient.app: main.o ${OBJS} ${LIBS}
	${CXX} ${LDFLAGS} -o $@ $^

include ../../Makefile.includes