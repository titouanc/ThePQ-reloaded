make.ok: network/test.ok pitch/test.ok json/test.ok model/test.ok
	touch $@

json/libjson.a: json
	make -C json

%/test.ok: % json/libjson.a
	make -C $<

clean:
	rm -f *.ok
	for dir in pitch network json model; do make -C $$dir clean; done

mrproper: clean
	for dir in pitch network json model; do make -C $$dir mrproper; done	