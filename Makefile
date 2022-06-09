all:
	cd src && make all
	mv src/kirscale kirscale;
clean:
	-rm kirscale
	-make rmdoc
	-cd src && make clean
run:
	./kirscale
compile:
	make all
	cd src && make clean
doc:
	doxygen Doxyfile
rmdoc:
	rm -r doc