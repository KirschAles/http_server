all:
	cd src && make all
	mv src/httpServ httpServ;
clean:
	-rm httpServ
	-make rmdoc
	-cd src && make clean
run:
	./httpServ
compile:
	make all
	cd src && make clean
doc:
	doxygen Doxyfile
rmdoc:
	rm -r doc