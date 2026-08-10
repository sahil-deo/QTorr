cmake_minimum_required(VERSION 3.20)

project (qtorr CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED)

add_executable(qtorr 
src/main.cpp
src/bencode.cpp
)

target_link_libraries(qtorr PRIVATE CURL::libcurl)

target_include_directories(qtorr PRIVATE ${CMAKE_SOURCE_DIR}/include)
