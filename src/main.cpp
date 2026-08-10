#include <iostream>
#include "bencode_parser.hpp"
#include "file.hpp"
#include "network.hpp"
#include "hash.hpp"

int main(int argc, char **argv)
{
    BencodeParser bp;

    // read .torrent file
    std::string filepath("/Users/sahil/Downloads/ubuntu-26.04-desktop-amd64.iso.torrent");
    // std::string filepath("/Users/sahil/Developer/C++/torrent client /test/test.torrent");
    std::string file = ReadFile(filepath);
    FileData fd(file);

    // decode the b-encoded file
    BValue result = bp.Decode(fd);

    // extract info string
    std::string hashInfoString = bp.GetHashInfo(result);

    // hash the info string into info hash
    std::string info_hash = HashSHA1(hashInfoString);

    // extract tracker url string
    std::string tracker_url = bp.GetTrackerUrl(result);
    PrintMessage(tracker_url);

    // populate the url with correct data

    std::string pid = EncodeHash(GeneratePeerId());
    std::string encoded_info_hash = EncodeHash(info_hash);
    std::string len = std::to_string(std::get<long long>(bp.GetKey(bp.GetKey(result, "info"), "length")));
    std::string port = "9999";
    std::string url = tracker_url + "?info_hash=" + encoded_info_hash + "&peer_id=" + pid + "&port=" + port + "&uploaded=0&downloaded=0" + "&left=" + len + "&compact=1&event=started";

    // make a get call
    PrintMessage(url);
    std::string response = Get(url);
    PrintMessage(response);

    // accept response

    // parse the response

    return 0;
}
