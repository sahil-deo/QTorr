#include <iostream>
#include "bencode_parser.hpp"
#include <map>
#include <vector>
BValue BencodeParser::Encode(FileData &file_data)
{
    return BValue();
}

BValue BencodeParser::Decode(FileData &file_data)
{
    char current = file_data.file[file_data.current_position];
    BValue result;
    std::string current_number = "";

    int n{};

    if (current == 'i') // integer
    {
        current_number = "";
        file_data.current_position++;
        current = file_data.file[file_data.current_position];
        while (current != 'e')
        {
            current_number += current;
            file_data.current_position++;
            current = file_data.file[file_data.current_position];
        }
        n = std::stoi(current_number);
        file_data.current_position++;
        result.emplace<int>(n);
    }
    else if (current == 'd') // dict
    {
        file_data.current_position++;
        current = file_data.file[file_data.current_position];

        std::string key;
        BValue value;
        std::map<std::string, BValue> mp;

        while (current != 'e')
        {
            key = std::get<std::string>(Decode(file_data));
            value = Decode(file_data);
            std::cout << key << "\n";
            mp[key] = value;
            current = file_data.file[file_data.current_position];
        }
        file_data.current_position++;
        result.emplace<std::map<std::string, BValue>>(mp);
    }
    else if (current == 'l') // list
    {
        std::vector<BValue> vec;

        file_data.current_position++;
        current = file_data.file[file_data.current_position];
        BValue value;
        while (current != 'e')
        {
            value = Decode(file_data);
            current = file_data.file[file_data.current_position];
            vec.push_back(value);
        }
        file_data.current_position++;
        result.emplace<std::vector<BValue>>(vec);
    }
    else // String
    {
        // First assume it is a digit, append to current size until ':'
        while (current != ':') // FIX: check if end of string to avoid out of bound error DONE
        {
            current_number += current;

            if (current > '9')
            {
                PrintError("current char not recognized, expect digit[0-9] got something else");
                PrintError("current_position: " + std::to_string(file_data.current_position));
                PrintError(std::string("current char: ") + current);
                abort();
            }

            file_data.current_position++;

            if (file_data.current_position >= file_data.file.size())
            {
                PrintMessage("current position > file size");
                PrintMessage("current_position: " + std::to_string(file_data.current_position));
                PrintMessage("file size: " + std::to_string(file_data.file.size()));
                abort();
            }

            current = file_data.file[file_data.current_position];
        }

        n = std::stoi(current_number);
        CheckBound(file_data.file.size(), file_data.current_position, n);

        std::string str = file_data.file.substr(file_data.current_position + 1, n);
        file_data.current_position = file_data.current_position + n + 1;
        result.emplace<std::string>(str);
    }

    return result;
}

void BencodeParser::Read(BValue &decoded)
{
    if (std::holds_alternative<std::map<std::string, BValue>>(decoded))
    {
        for (auto &e : std::get<std::map<std::string, BValue>>(decoded))
        {
            std::cout << e.first << ": ";
            Read(e.second);
            std::cout << std::endl;
        }
    }
    else if (std::holds_alternative<std::vector<BValue>>(decoded))
    {
        for (auto &e : std::get<std::vector<BValue>>(decoded))
        {
            Read(e);
            std::cout << std::endl;
        }
    }
    else if (std::holds_alternative<int>(decoded))
    {
        std::cout << std::get<int>(decoded) << std::endl;
    }
    else if (std::holds_alternative<std::string>(decoded))
    {
        std::cout << std::get<std::string>(decoded) << std::endl;
    }
}

void BencodeParser::CheckBound(int file_size, int current_position, int n)
{
    if (current_position + 1 + n > file_size)
    {
        PrintError("Size of string out of bounds");
        PrintError(std::string("file size: " + std::to_string(file_size)));
        PrintError(std::string("position calculated bound: " + std::to_string(current_position + 1 + n)));
        abort();
    }
}