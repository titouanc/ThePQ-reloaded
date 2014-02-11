all: json/libjson.a network/libnetwork.a pitch/test

json/libjson.a:
	make -C json

network/libnetwork.a: json/libjson.a
	make -C network

pitch/test: json/libjson.a
	make -C pitch
