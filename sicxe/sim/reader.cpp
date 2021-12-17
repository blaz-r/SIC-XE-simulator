#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>

std::string readString(std::iostream& r, int len)
{
    char str[len];
    r.read(str, len);
    return std::string(str, len);
}

uint8_t readByte(std::iostream& r)
{
    char str[3] = {0};
    r.read(str, 2);
    return std::stoul(str, nullptr, 16);
}

int32_t readWord(std::iostream& r)
{
    char str[7] = {0};
    r.read(str, 6);
    return std::stoul(str, nullptr, 16);
}
