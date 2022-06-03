CC = g++
CFLAGS = -Wall --pedantic -g -std=c++17

clean:
	rm *.o;
	rm *.exe

all: main.exe
main.exe: main.o configuration.o networking.o
	$(CC) $(CFLAGS) -o main.exe main.o configuration.o networking.o
networking.o: networking.cpp
	$(CC) $(CFLAGS) -c networking.cpp -o networking.o


configuration.o: configuration.cpp
	$(CC) $(CFLAGS) -c configuration.cpp -o configuration.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o



