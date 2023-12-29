#include "../include/huffdecode.h"
#include "../include/huffencode.h"

using namespace huffman;

void testHuffTree(const std::string &filename) {
    size_t b = filename.find_last_of('.');
    huffman::file = filename.substr(0, b);
    std::string destFilename = huffman::file + ".code";
    huffEncode he;
    he.encode(filename.data(), destFilename.data());
    he.compreRatio(filename.data());
}

void testHuffDecode(const std::string &filename) {
    size_t b = filename.find_last_of('.');
    std::string destFilename = filename.substr(0, b) + ".decode";
    huffDecode hd;
    hd.decode(filename.data(), destFilename.data());
}

void manual() {
    std::ifstream manfile("../include/manual.txt");

    if (!manfile.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(manfile, line)) {
        std::cout << line << std::endl;
    }
    manfile.close();
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<file_name>"
                  << " <command_line_argument>" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "en" | arg == "de") {
            i++;
            continue;
        }
        if (arg == "--file") huffman::flag |= FLAG_FILE;
        else if (arg == "--all")
            huffman::flag |= FLAG_ALL;
        else if (arg == "--map")
            huffman::flag |= FLAG_MAP;
        else if (arg == "--tree")
            huffman::flag |= FLAG_TREE;
        else if (arg == "--help") {
            manual();
            return 0;
        } else {
            std::cerr << "Usage: "
                      << "不支持选项：" << argv[i]
                      << "\n请尝试执行 './a.out -- help ' 来获取更多信息。" << std::endl;
            return 2;
        }
    }
    std::string mode = argv[1];
    std::string filename = argv[2];

    if (mode == "en") testHuffTree(filename);
    if (mode == "de") testHuffDecode(filename);
    return 0;
}