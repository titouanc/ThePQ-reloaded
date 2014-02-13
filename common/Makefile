all: json/test.ok network/test.ok pitch/test.ok

json/test.ok: json
	make -C json

network/test.ok: json/libjson.a network
	make -C network

pitch/test.ok: json/libjson.a pitch
	make -C pitch

clean:
	make -C json clean
	make -C pitch clean
	make -C network clean
