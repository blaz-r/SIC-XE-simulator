#ifndef READER_H
#define READER

#include <iostream>
#include <string>
#include <stdint.h>

std::string readString(std::iostream& r, int len);
uint8_t readByte(std::iostream& r);
int32_t readWord(std::iostream& r);

#endif