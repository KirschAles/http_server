CC = g++
CFLAGS = -Wall --pedantic -g -std=c++17


all: parserTest.exe
parserTest.exe: parserTest.o FileParser.o FileParserBinary.o FileParserRegular.o FileParserText.o FileParserScript.o FileParserDirectory.o
	$(CC) $(CFLAGS) -o parserTest.exe parserTest.o FileParser.o FileParserBinary.o FileParserRegular.o FileParserText.o FileParserScript.o FileParserDirectory.o -lstdc++fs;
	rm *.o
parserTest.o:
	$(CC) $(CFLAGS) -c parserTest.cpp -o parserTest.o
FileParser.o:
	$(CC) $(CFLAGS) -c FileParser.cpp -o FileParser.o
FileParserRegular.o:
	$(CC) $(CFLAGS) -c FileParserRegular.cpp -o FileParserRegular.o
FileParserBinary.o:
	$(CC) $(CFLAGS) -c FileParserBinary.cpp -o FileParserBinary.o
FileParserText.o:
	$(CC) $(CFLAGS) -c FileParserText.cpp -o FileParserText.o
FileParserScript.o:
	$(CC) $(CFLAGS) -c FileParserScript.cpp -o FileParserScript.o
FileParserDirectory.o:
	$(CC) $(CFLAGS) -c FileParserDirectory.cpp -o FileParserDirectory.o

clean:
	rm *.o *.exe