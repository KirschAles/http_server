all:
	cd src && make all
	mv src/HttpServer.exe HttpServer.exe;
clean:
	rm *.exe;
	cd src && make clean;
run:
	./HttpServer.exe
compile:
	cd src && make all
	mv src/HttpServer.exe HttpServer.exe
	cd src && make clean