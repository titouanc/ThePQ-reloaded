all: server client

.PHONY: common server client mrproper clean all

common:
	make -C common

server: common
	make -C server

client: common
	make -C client

clean:
	rm -f *.ok
	for dir in server client common; do make -C $$dir clean; done

mrproper: clean
	for dir in server client common; do make -C $$dir mrproper; done
