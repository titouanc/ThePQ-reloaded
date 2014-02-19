all: json model network

.PHONY: all clean mrproper json model network

json:
	make -C json

model: json
	make -C model

network: json
	make -C network

clean:
	rm -f *.ok
	for dir in network json model; do make -C $$dir clean; done

mrproper: clean
	for dir in network json model; do make -C $$dir mrproper; done	