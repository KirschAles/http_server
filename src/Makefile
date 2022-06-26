CC = g++
CFLAGS = -Wall --pedantic -std=c++17


all: httpServ
httpServ: HttpServer.o configuration.o contentGenerator.o communication.o connection.o httpConnection.o server.o fileParser.o fileParserScript.o fileParserDirectory.o fileParserRegular.o fileParserBinary.o fileParserText.o loggable.o request.o getRequest.o simpleResponse.o fullResponse.o fullErrorResponse.o simpleErrorResponse.o logger.o fileParserImage.o
	$(CC) $(CFLAGS) -o httpServ build/HttpServer.o build/configuration.o build/contentGenerator.o build/communication.o build/connection.o build/httpConnection.o build/server.o build/fileParserImage.o build/fileParser.o build/fileParserScript.o build/fileParserDirectory.o build/fileParserRegular.o build/fileParserBinary.o build/fileParserText.o build/loggable.o build/request.o build/getRequest.o build/simpleResponse.o build/fullResponse.o build/fullErrorResponse.o build/simpleErrorResponse.o build/logger.o -lstdc++fs -pthread
configuration.o: build configuration.cpp
	$(CC) $(CFLAGS) -c configuration.cpp -o build/configuration.o
HttpServer.o: build HttpServer.cpp
	$(CC) $(CFLAGS) -c HttpServer.cpp -o build/HttpServer.o
contentGenerator.o: build ContentGenerator.cpp
	$(CC) $(CFLAGS) -c ContentGenerator.cpp -o build/contentGenerator.o
communication.o: build communicationManagement/Communication.cpp
	$(CC) $(CFLAGS) -c communicationManagement/Communication.cpp -o build/communication.o
connection.o: build communicationManagement/Connection.cpp
	$(CC) $(CFLAGS) -c communicationManagement/Connection.cpp -o build/connection.o
httpConnection.o: build communicationManagement/HttpConnection.cpp
	$(CC) $(CFLAGS) -c communicationManagement/HttpConnection.cpp -o build/httpConnection.o
server.o: build communicationManagement/Server.cpp
	$(CC) $(CFLAGS) -c communicationManagement/Server.cpp -o build/server.o
fileParser.o: build fileParsers/FileParser.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParser.cpp -o build/fileParser.o
fileParserScript.o: build fileParsers/FileParserScript.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserScript.cpp -o build/fileParserScript.o
fileParserDirectory.o: build fileParsers/FileParserDirectory.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserDirectory.cpp -o build/fileParserDirectory.o
fileParserRegular.o: build fileParsers/FileParserRegular.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserRegular.cpp -o build/fileParserRegular.o
fileParserBinary.o: build fileParsers/FileParserBinary.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserBinary.cpp -o build/fileParserBinary.o
fileParserText.o: build fileParsers/FileParserText.cpp
	$(CC) $(CFLAGS) -c fileParsers/FileParserText.cpp -o build/fileParserText.o
fileParserImage.o: build fileParsers/FileParserImage.cpp fileParsers/FileParserImage.h
	$(CC) $(CFLAGS) -c fileParsers/FileParserImage.cpp -o build/fileParserImage.o
loggable.o: build loggable/Loggable.cpp
	$(CC) $(CFLAGS) -c loggable/Loggable.cpp -o build/loggable.o
request.o: build loggable/requests/Request.cpp
	$(CC) $(CFLAGS) -c loggable/requests/Request.cpp -o build/request.o
getRequest.o: build loggable/requests/GETRequest.cpp
	$(CC) $(CFLAGS) -c loggable/requests/GETRequest.cpp -o build/getRequest.o
simpleResponse.o: build loggable/responses/SimpleResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/SimpleResponse.cpp -o build/simpleResponse.o
fullResponse.o: build loggable/responses/FullResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/FullResponse.cpp -o build/fullResponse.o
fullErrorResponse.o: build loggable/responses/FullErrorResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/FullErrorResponse.cpp -o build/fullErrorResponse.o
simpleErrorResponse.o: build loggable/responses/SimpleErrorResponse.cpp
	$(CC) $(CFLAGS) -c loggable/responses/SimpleErrorResponse.cpp -o build/simpleErrorResponse.o
logger.o: build Logger.cpp Logger.h
	$(CC) $(CFLAGS) -c Logger.cpp -o build/logger.o
build:
	mkdir build
clean:
	rm -r build


