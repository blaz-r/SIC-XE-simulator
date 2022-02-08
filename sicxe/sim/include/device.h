#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <QObject>

class Device
{
public:
    Device() {}
    ~Device() {}

    virtual bool test() { return true; }
    virtual uint8_t read() { return 0; }
    virtual void write(uint8_t) {}
};

class InputDevice: public Device
{
private:
    std::istream& inputStream;
public:
    InputDevice(std::istream& in): inputStream(in) {}
    ~InputDevice() {}

    uint8_t read() override;
};

class OutputDevice: public Device
{
private:
    std::ostream& outputStream;
public:
    OutputDevice(std::ostream& out): outputStream(out) {}
    ~OutputDevice() {}

    void write(uint8_t value) override;
};

class FileDevice: public Device
{
private:
    std::fstream fileStream;
public:
    FileDevice(std::string file);
    ~FileDevice();

    uint8_t read() override;
    void write(uint8_t value) override;
    void reset();
};

#endif
