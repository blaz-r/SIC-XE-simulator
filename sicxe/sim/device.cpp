#include "device.h"
#include <iostream>

uint8_t InputDevice::read()
{
    return inputStream.get();
}


void OutputDevice::write(uint8_t value)
{
    outputStream.write(reinterpret_cast<char*>(&value), 1);
    outputStream.flush();
}

FileDevice::FileDevice(std::string file)
{
    fileStream.open(file, std::fstream::in);
    if(!fileStream)
    {
        fileStream.open(file, std::fstream::out);
    }
    fileStream.close();
    fileStream.open(file, std::fstream::out | std::fstream::in);
}

FileDevice::~FileDevice()
{ 
    fileStream.close();
}

uint8_t FileDevice::read()
{
    if (fileStream.peek() == EOF)
    {
        return 0;
    }
    else
    {
        return fileStream.get();
    }
}

void FileDevice::write(uint8_t value)
{
    fileStream.write(reinterpret_cast<char*>(&value), 1);
    fileStream.flush();
}

void FileDevice::reset()
{
    fileStream.clear();
    fileStream.seekg(0);
}
