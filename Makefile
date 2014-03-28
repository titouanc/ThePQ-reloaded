all: server client

.PHONY: common server client mrproper clean all

client/graphicClient.app: client

ThePQ.app: client/graphicClient.app
	rm -rf $@
	mkdir -p $@/Contents/MacOS/graphics $@/Contents/Frameworks
	#cp -r client/graphics/sounds client/graphics/textures client/graphics/fonts $@/Contents/MacOS/graphics/
	cp $< $@/Contents/MacOS/ThePQ
	echo '{"host": "thepq.partou.se", "port": 32123}' > $@/Contents/MacOS/netconfig.json
	cp -R /Library/Frameworks/SFML.framework $@/Contents/Frameworks

common:
	+make -C common

server: common
	+make -C server

client: common
	+make -C client

clean:
	rm -f *.ok
	for dir in server client common; do make -C $$dir clean; done

mrproper: clean
	for dir in server client common; do make -C $$dir mrproper; done
