CC = g++
CFLAGS = -Wall --pedantic -g -std=c++17

clean:
	rm *.o;
	rm *.exe

all: main.exe
main.exe: main.o configuration.o contentGenerator.o communication.o connection.o httpConnection.o server.o fileParser.o fileParserScript.o fileParserDirectory.o fileParserRegular.o fileParserBinary.o fileParserText.o loggable.o request.o getRequest.o simpleResponse.o fullResponse.o fullErrorResponse.o simpleErrorResponse.o
	$(CC) $(CFLAGS) -o main.exe main.o configuration.o contentGenerator.o communication.o connection.o httpConnection.o server.o fileParser.o fileParserScript.o fileParserDirectory.o fileParserRegular.o fileParserBinary.o fileParserText.o loggable.o request.o getRequest.o simpleResponse.o fullResponse.o fullErrorResponse.o simpleErrorResponse.o -lstdc++fs
configuration.o: configuration.cpp
	$(CC) $(CFLAGS) -c configuration.cpp -o configuration.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o
codes.o: constants/codes.cpp
	$(CC) $(CFLAGS) -c constants/codes.cpp -o codes.o
contentGenerator.o: ContentGenerator.cpp
	$(CC) $(CFLAGS) -c ContentGenerator.cpp -o contentGenerator.o
communication.o: communicationManagement/Communication.cpp
	$(CC) $(CFLAGS) -c communicationManagement/Communication.cpp -o communication.o
connection.o: communicationManagement/Connection.cpp
	$(CC) $(CFLAGS) -c communicationManagement/Connection.cpp -o connection.o
httpConnection.o: communicationManagement/HttpConnection.cpp
	$(CC) $(CFLAGS) -c communicationManagement/HttpConnection.cpp -o httpConnection.o
server.o: communicationManagement/Server.cpp
	$(CC) $(CFLAGS) -c communicationManagement/Server.cpp -o server.o
fileParser.o: fileParsers/FileParser.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParser.cpp -o fileParser.o
fileParserScript.o: fileParsers/FileParserScript.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserScript.cpp -o fileParserScript.o
fileParserDirectory.o: fileParsers/FileParserDirectory.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserDirectory.cpp -o fileParserDirectory.o
fileParserRegular.o: fileParsers/FileParserRegular.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserRegular.cpp -o fileParserRegular.o
fileParserBinary.o: fileParsers/FileParserBinary.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserBinary.cpp -o fileParserBinary.o
fileParserText.o: fileParsers/FileParserText.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserText.cpp -o fileParserText.o
loggable.o: loggable/Loggable.cpp
	$(CC) $(CFLAGS) -c loggable/Loggable.cpp -o loggable.o
request.o: loggable/requests/Request.cpp
	$(CC) $(CFLAGS) -c loggable/requests/Request.cpp -o request.o
getRequest.o: loggable/requests/GETRequest.cpp
	$(CC) $(CFLAGS) -c loggable/requests/GETRequest.cpp -o getRequest.o
simpleResponse.o: loggable/responses/SimpleResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/SimpleResponse.cpp -o simpleResponse.o
fullResponse.o: loggable/responses/FullResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/FullResponse.cpp -o fullResponse.o
fullErrorResponse.o: loggable/responses/FullErrorResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/FullErrorResponse.cpp -o fullErrorResponse.o
simpleErrorResponse.o: loggable/responses/SimpleErrorResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/SimpleErrorResponse.cpp -o SimpleErrorResponse.o


