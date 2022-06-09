all:
	cd src && make all
	mv src/main.exe main.exe;
clean:
	rm main.exe;
	cd src && make clean;
run:
	./main.exe
compile:
	cd src && make all
	mv src/main.exe main.exe
	cd src && make clean