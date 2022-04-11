CC = g++
CFLAGS = -Wall --pedantic -g

clean:
	rm *.o;
	rm *.exe

all: main.exe
main.exe: main.o
	$(CC) $(CFLAGS) -o main.exe main.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.o main.cpp

