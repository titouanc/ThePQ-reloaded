SUBDIRS = json model network tests
all: ${SUBDIRS}

.PHONY: all clean mrproper ${SUBDIRS}

json:
	+make -C $@

tests:
	+make -C $@

model: json
	+make -C $@

network: json
	+make -C $@

clean:
	rm -f *.ok
	for dir in ${SUBDIRS}; do make -C $$dir clean; done

mrproper: clean
	for dir in ${SUBDIRS}; do make -C $$dir mrproper; done
