#include "external/TinySHA1.hpp"
#include <string>
#include <cstdint>
#include <iomanip>
#include <random>
#include <sstream>
#include <curl/curl.h>
std::string HashSHA1(const std::string &encoded_info)
{
    sha1::SHA1 s;
    s.processBytes(encoded_info.data(), encoded_info.size());

    uint32_t digest[5];
    s.getDigest(digest);

    std::string hash;
    hash.reserve(20);
    for (int i = 0; i < 5; ++i)
    {
        hash += static_cast<char>((digest[i] >> 24) & 0xFF);
        hash += static_cast<char>((digest[i] >> 16) & 0xFF);
        hash += static_cast<char>((digest[i] >> 8) & 0xFF);
        hash += static_cast<char>(digest[i] & 0xFF);
    }
    return hash;
}

std::string ToHex(const std::string &raw)
{
    std::ostringstream oss;
    for (unsigned char c : raw)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return oss.str();
}

std::string GeneratePeerId()
{
    std::string prefix = "-QT0001-"; // "QT" = your client, "0001" = version
    std::string peer_id = prefix;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    while (peer_id.size() < 20)
    {
        peer_id += static_cast<char>(dist(gen));
    }
    return peer_id;
}

std::string EncodeHash(std::string hash)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        abort();
    }

    char *escaped = curl_easy_escape(curl, hash.data(), static_cast<int>(hash.size()));
    std::string encoded(escaped);

    curl_free(escaped);
    curl_easy_cleanup(curl);

    return encoded;
}