#include "../include/huffdecode.h"
#include "../include/huffencode.h"

using namespace huffman;

std::string testHuffTree(const std::string& filename) {
    size_t b = filename.find_last_of('.');
    huffman::file = filename.substr(0, b);
    std::string destFilename = filename.substr(0, b) + ".code";
    huffEncode he;
    he.encode(filename.data(), destFilename.data());
    he.compreRatio(filename.data());
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
        std::cerr << "Usage: " << argv[0] <<"<file_name>" <<" <command_line_argument>" << std::endl;
        return 1;
    }

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--file") huffman::flag |= FLAG_FILE;
        if (arg == "--all") huffman::flag |= FLAG_ALL;
        if (arg == "--map") huffman::flag |= FLAG_MAP;
        if (arg == "--tree") huffman::flag |= FLAG_TREE;
    }

    std::string filename = argv[1];
    std::string destFilename = testHuffTree(filename);
    testHuffDecode(destFilename);

    return 0;
}