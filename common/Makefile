make.ok: network/test.ok pitch/test.ok
	touch $@

json/libjson.a: json
	make -C json

%/test.ok: % json/libjson.a
	make -C $<

clean:
	rm -f *.ok
	for dir in pitch network json; do make -C $$dir clean; done

mrproper: clean
	for dir in pitch network json; do make -C $$dir mrproper; done	