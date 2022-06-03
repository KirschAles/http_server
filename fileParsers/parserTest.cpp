#include "FileParserText.h"
#include "FileParserScript.h"
#include "FileParserDirectory.h"
#include "FileParserBinary.h"






int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Format ./<program name> <test file>" << std::endl;
        return -1;
    }
    std::string chunk;
    fs::path  paat(argv[1]);
    paat = fs::absolute(paat);

    FileParserBinary parser(paat, 100);
    std::cout << parser.getSize() << std::endl;
    while (!parser.isEmpty()) {
        chunk = parser.getChunk();
        std::cout << chunk;
    }
    return 0;
}

