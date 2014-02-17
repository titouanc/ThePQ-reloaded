make.ok: json/libjson.a network/libnetwork.a model/libmodel.a
	touch $@

json/libjson.a: json
	make -C $<

network/libnetwork.a: network json/libjson.a
	make -C $<

model/libmodel.a: model json/libjson.a
	make -C $<

clean:
	rm -f *.ok
	for dir in pitch network json model; do make -C $$dir clean; done

mrproper: clean
	for dir in pitch network json model; do make -C $$dir mrproper; done	