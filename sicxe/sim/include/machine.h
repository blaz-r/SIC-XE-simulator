#ifndef MACHINE_H
#define MACHINE_H

#include <cstdint>
#include "device.h"
#include "registers.h"
#include <array>
#include <memory>
#include <QObject>

#define MAX_ADDRESS 1048575
#define HALT 42

class Machine : public QObject
{
    Q_OBJECT

private:
    std::array<std::shared_ptr<Device>, 256> devices = {nullptr};
    std::string currentFile = "";
public:
    uint8_t* memory;

    Registers reg;

    Machine();
    ~Machine();

    enum AccessSize {s_Byte=1, s_Word=3, s_Float=6};

    int assertValidAddress(uint32_t addr, AccessSize size);

    int8_t getByte(uint32_t addr);
    void setByte(uint32_t addr, int8_t value);

    int32_t getWord(uint32_t addr);
    void setWord(uint32_t addr, int32_t value);

    double getFloat(uint32_t addr);
    void setFloat(uint32_t addr, double value);

    std::shared_ptr<Device> getDevice(uint8_t num);
    void setDevice(uint8_t num, std::shared_ptr<Device> device);

    void initInOutErr();

    void notImplemented(std::string mnemonic);
    void invalidOpcode(int opcode);
    void invalidAddressing(std::string details);

    bool execF1(int opcode);
    bool execF2(int opcode, int operand);

    int32_t loadWord(int ni, int operand);
    uint8_t loadByte(int ni, int operand);
    double loadFloat(int ni, double operand);

    uint32_t storeAddress(int ni, int operand);

    int execSICF3F4(int opcode, int ni, int operand);

    uint8_t fetch();
    int execute();

    void reset();

    bool loadSection(std::string fileName);

signals:
    Q_SIGNAL void valueWritten(std::pair<uint32_t, int64_t> addrValuePair);
    Q_SIGNAL void floatWritten(std::pair<uint32_t, double> addrValuePair);
};
#endif
