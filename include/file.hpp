#include <fstream>
#include <filesystem>

std::string ReadFile(std::filesystem::path filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    auto file_size = std::filesystem::file_size(filePath);
    std::string fd(file_size, '\0');

    file.read(&fd[0], file_size);
    return fd;
}