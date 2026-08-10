#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include "log.hpp"

struct BValue : std::variant<long long, std::string, std::vector<BValue>, std::map<std::string, BValue>>
{
};

struct FileData
{
    FileData(std::string file) : file(file) {}
    std::string file;
    size_t current_position = 0;

    bool CheckBound() { return (current_position >= file.size()); }
};

class BencodeParser
{

public:
    BencodeParser() {};

    BValue Encode(FileData &file_data);

    std::string GetHashInfo(BValue &decoded);

    std::string GetTrackerUrl(BValue &decoded);

    BValue GetKey(BValue decoded, std::string key);

    BValue Decode(FileData &file_data);

    void Read(BValue &decoded);

private:
    std::string EncodeFromBValue(BValue &file_data);
    void CheckBound(long long file_size, int current_position, int n);
};
