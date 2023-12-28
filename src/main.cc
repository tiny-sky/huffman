#include "../include/huffdecode.h"
#include "../include/huffencode.h"

using namespace huffman;

std::string testHuffTree(const std::string& filename) {
    size_t b = filename.find_last_of('.');
    std::string destFilename = filename.substr(0, b) + ".code";
    huffEncode he;
    he.encode(filename.data(), destFilename.data());
    return destFilename;
}

void testHuffDecode(const std::string& filename) {
    size_t b = filename.find_last_of('.');
    std::string destFilename = filename.substr(0, b) + ".decode";
    huffDecode hd;
    hd.decode(filename.data(), destFilename.data());
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command_line_argument>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string destFilename = testHuffTree(filename);
    testHuffDecode(destFilename);

    return 0;
}