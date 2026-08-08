#include <iostream>
#include "bencode_parser.hpp"
#include "file.hpp"
int main(int argc, char **argv)
{
    BencodeParser bp;

    std::string filepath("/Users/sahil/Downloads/F6D61E9ECECACF88FF6F956EBA752FBBBEFCDD27.torrent");
    std::string file = ReadFile(filepath);
    FileData fd(file);

    BValue result = bp.Decode(fd);
    bp.Read(result);
}
