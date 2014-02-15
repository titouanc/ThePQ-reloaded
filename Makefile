make.ok: server/make.ok client/make.ok
	touch $@
	uname -a
	${CXX} --version

common/make.ok: common
	make -C $<

%/make.ok: % common/make.ok
	make -C $<

clean:
	rm -f *.ok
	for dir in server client common; do make -C $$dir clean; done

mrproper: clean
	for dir in server client common; do make -C $$dir mrproper; done
