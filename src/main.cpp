#include <iostream>
#include "bencode_parser.hpp"
#include "file.hpp"
int main(int argc, char **argv)
{
    BencodeParser bp;

    std::string filepath("/Users/sahil/Developer/C++/torrent client /test/test.torrent");
    std::string file = ReadFile(filepath);
    FileData fd(file);

    BValue result = bp.Decode(fd);

    std::string reencoded = bp.EncodeFromBValue(result);

    std::cout << reencoded << "\n";

    return 0;
}
