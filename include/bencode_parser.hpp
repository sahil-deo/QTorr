#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include "log.hpp"

struct BValue : std::variant<int, std::string, std::vector<BValue>, std::map<std::string, BValue>>
{
    void PrintInt()
    {
        std::cout << std::get<int>(*this) << std::endl;
    }

    void PrintStr()
    {
        std::cout << std::get<std::string>(*this) << std::endl;
    }
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

    std::string EncodeFromBValue(BValue &file_data);

    BValue Decode(FileData &file_data);

    void Read(BValue &decoded);

private:
    void CheckBound(int file_size, int current_position, int n);
};
