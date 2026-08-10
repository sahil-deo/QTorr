#include <iostream>
#include "bencode_parser.hpp"
#include <map>
#include <vector>
BValue BencodeParser::Encode(FileData &file_data)
{
    return BValue();
}

std::string BencodeParser::GetHashInfo(BValue &decoded)
{
    BValue info = GetKey(decoded, "info");
    return EncodeFromBValue(info);
}

std::string BencodeParser::GetTrackerUrl(BValue &decoded)
{
    std::string trackerUrl;
    for (auto &e : std::get<std::map<std::string, BValue>>(decoded))
    {
        if (e.first == "announce")
        {
            return std::get<std::string>(e.second);
        }
    }
    return "";
}

BValue BencodeParser::GetKey(BValue decoded, std::string key)
{
    std::string trackerUrl;
    if (!std::holds_alternative<std::map<std::string, BValue>>(decoded))
    {
        std::cout << "NOT A DICT\n";
        abort();
    }

    for (auto &e : std::get<std::map<std::string, BValue>>(decoded))
    {
        if (e.first == key)
        {
            return e.second;
        }
    }
    std::cout << "KEY NOT FOUND: " << key << "\n";
    abort();
}

std::string BencodeParser::EncodeFromBValue(BValue &file_data)
{
    std::string result{};

    if (std::holds_alternative<std::map<std::string, BValue>>(file_data))
    {
        result += "d";
        for (auto &e : std::get<std::map<std::string, BValue>>(file_data))
        {
            std::string s = e.first;
            result += std::to_string(s.size());
            result += ":";
            result += s;
            result += EncodeFromBValue(e.second);
        }
        result += "e";
    }
    else if (std::holds_alternative<std::vector<BValue>>(file_data))
    {
        result += "l";
        for (auto &e : std::get<std::vector<BValue>>(file_data))
        {
            result += EncodeFromBValue(e);
        }
        result += "e";
    }
    else if (std::holds_alternative<long long>(file_data))
    {
        result += "i";
        result += std::to_string(std::get<long long>(file_data));
        result += "e";
    }
    else if (std::holds_alternative<std::string>(file_data))
    {
        std::string s = std::get<std::string>(file_data);
        result += std::to_string(s.size());
        result += ":";
        result += s;
    }
    return result;
}

BValue BencodeParser::Decode(FileData &file_data)
{
    char current = file_data.file[file_data.current_position];
    BValue result{};
    std::string current_number = {};

    long long n{};

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
        n = std::stoll(current_number);
        file_data.current_position++;
        result.emplace<long long>(n);
    }
    else if (current == 'd') // dict
    {
        file_data.current_position++;
        current = file_data.file[file_data.current_position];

        std::string key{};
        BValue value{};
        std::map<std::string, BValue> mp{};

        while (current != 'e')
        {
            key = std::get<std::string>(Decode(file_data));
            value = Decode(file_data);
            mp[key] = value;
            current = file_data.file[file_data.current_position];
        }
        file_data.current_position++;
        result.emplace<std::map<std::string, BValue>>(mp);
    }
    else if (current == 'l') // list
    {
        std::vector<BValue> vec{};

        file_data.current_position++;
        current = file_data.file[file_data.current_position];
        BValue value{};
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
    if (std::holds_alternative<std::map<std::string, BValue>>(decoded)) // map
    {
        for (auto &e : std::get<std::map<std::string, BValue>>(decoded))
        {
            std::cout << e.first << ": ";
            Read(e.second);
            std::cout << std::endl;
        }
    }
    else if (std::holds_alternative<std::vector<BValue>>(decoded)) // list
    {
        for (auto &e : std::get<std::vector<BValue>>(decoded))
        {
            Read(e);
            std::cout << std::endl;
        }
    }
    else if (std::holds_alternative<long long>(decoded)) // int
    {
        std::cout << std::get<long long>(decoded) << std::endl;
    }
    else if (std::holds_alternative<std::string>(decoded)) // string
    {
        std::cout << std::get<std::string>(decoded) << std::endl;
    }
}

void BencodeParser::CheckBound(long long file_size, int current_position, int n)
{
    if (current_position + 1 + n > file_size)
    {
        PrintError("Size of string out of bounds");
        PrintError(std::string("file size: " + std::to_string(file_size)));
        PrintError(std::string("position calculated bound: " + std::to_string(current_position + 1 + n)));
        abort();
    }
}