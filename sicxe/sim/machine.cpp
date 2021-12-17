#include <iostream>
#include <memory>
#include <string.h>
#include <sstream>
#include "machine.h"
#include "device.h"
#include "opcode.h"
#include "reader.h"


Machine::Machine()
{
    memory = new uint8_t[MAX_ADDRESS + 1];

    initInOutErr();
}

Machine::~Machine()
{
    delete[] memory;
}

int Machine::assertValidAddress(uint32_t addr, AccessSize size)
{
    if(size == s_Byte && addr <= MAX_ADDRESS)
    {
        return 1;
    }
    else if(size == s_Word && addr <= MAX_ADDRESS - 2)
    {
        return 1;
    }
    else if(size == s_Float && addr <= MAX_ADDRESS - 5)
    {
        return 1;
    }
    std::cout << "Address: " << addr << " out of range for size: " << size << ", max address for this size: " << MAX_ADDRESS - size + 1 << std::endl;
    return 0;
}

int8_t Machine::getByte(uint32_t addr)
{
    return memory[addr];
}

void Machine::setByte(uint32_t addr, int8_t value)
{
    if(assertValidAddress(addr, s_Byte))
    {
        memory[addr] = value;
        emit valueWritten(std::make_pair(addr, value));
    }
}

int32_t Machine::getWord(uint32_t addr)
{
    int32_t word = memory[addr] << 16;
    word += memory[addr + 1] << 8;
    word += memory[addr + 2];

    if(word & (1 << 23)) {
        // negative
        word |= (255 << 24);
    }
    else 
    {
        // positive
        word &= ~(255 << 24);
    }

    return word;
}

void Machine::setWord(uint32_t addr, int32_t value)
{
    if(assertValidAddress(addr, s_Word))
    {
        memory[addr] = value >> 16;   
        memory[addr + 1] = value >> 8;
        memory[addr + 2] = value;

        emit valueWritten(std::make_pair(addr, value));
    }
}

bool is_big_endian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {1};

    return bint.c[0] == 0; 
}

double Machine::getFloat(uint32_t addr)
{
    double right_value = 0;

    if(is_big_endian())
    {
        /*
            in big endian we just copy 6 byte float to 6 top bytes of double
        */
        memcpy(&right_value, &memory[addr], 6);
    }
    else
    {
        /*
            in little endian we need to prepend zeros, since format is flipped
            it means we are basically setting last 16 bits of mantisa to 0
        */
        uint8_t from_mem[8] = {0};
        // this used to work on linux, mingw decied otherwise :(
//        memcpy(&from_mem[2], &memory[addr], 6);
        for(int i = 0; i < 6; i++)
        {
            from_mem[7-i] = memory[addr+i];
        }

        memcpy(&right_value, from_mem, 8);
    }
    return right_value;
}

void Machine::setFloat(uint32_t addr, double value)
{
    if(assertValidAddress(addr, s_Float))
    {
        uint8_t double_bits[8] = {0};
        memcpy(&double_bits, &value, sizeof(value));

        for(int i = 0; i < 6; i++)
        {
            if(is_big_endian())
            {
                /* 
                    when dealing with big endian we take first 6 bytes and cut off last 2 bytes of mantisa
                    BE: sign|exp|mantisa
                */
                memcpy(&memory[addr], double_bits, 6);
            }
            else
            {
                /* 
                    little endian is flipped so we need 6 bytes in reverse, cutting of 2 bytes of mantisa by skippin 2 bytes in front
                    LE: mantisa|exp|sign
                */
                // mingw says nope
//                memcpy(&memory[addr], &double_bits[2], 6);
                for(int i = 0; i < 6; i++)
                {
                    memory[addr+i] = double_bits[7-i];
                }
            }
        }

        emit floatWritten(std::make_pair(addr, value));
    }
}

std::shared_ptr<Device> Machine::getDevice(uint8_t num)
{
    if(devices[num] == nullptr)
    {
        std::stringstream ss;
        ss << std::hex << int(num);
        std::string name ( ss.str() + ".dev");
        setDevice(num, std::shared_ptr<Device>( new FileDevice(name)));
    }
    return devices[num];
}

void Machine::setDevice(uint8_t num, std::shared_ptr<Device> device)
{
    devices[num] = device;
}

void Machine::initInOutErr()
{
    setDevice(0, std::shared_ptr<Device>( new InputDevice(std::cin)));
    setDevice(1, std::shared_ptr<Device>( new OutputDevice(std::cout)));
    setDevice(2, std::shared_ptr<Device>( new OutputDevice(std::cerr)));
}

void Machine::notImplemented(std::string mnemonic)
{
    std::cout << mnemonic << " is not implemented!" << std::endl;
}

void Machine::invalidOpcode(int opcode)
{
    std::cout << opcode << " is invalid!" << std::endl;
}

void Machine::invalidAddressing(std::string details)
{
    std::cout << "Invlaid addressing, details: " << details << std::endl;
}

bool Machine::execF1(int opcode)
{
    switch (opcode)
    {
    case Opcode::FIX: 
        reg.setA((int32_t) reg.getF()); break;
    case Opcode::FLOAT: 
        reg.setF((double) reg.getA()); break;
    case Opcode::HIO:
        notImplemented("HIO"); break;
    case Opcode::NORM:
        notImplemented("NORM"); break;
    case Opcode::SIO:
        notImplemented("SIO"); break;
    case Opcode::TIO:
        notImplemented("TIO"); break; 
    default:
        return false;
    }
    
    return true;
}

bool Machine::execF2(int opcode, int operand)
{
    int r1 = operand >> 4;
    int r2 = operand & ~(15 << 4);

    int32_t r1Value, r2Value;

    // get registers values only if they are valid
    if(r1 <= Registers::SW && r2 <= Registers::SW)
    {
        r1Value = reg.getReg(r1);
        r2Value = reg.getReg(r2);
    }

    switch (opcode)
    {
    case Opcode::ADDR:
        reg.setReg(r2, r1Value + r2Value); break;
    case Opcode::CLEAR:
        reg.setReg(r1, 0); break;
    case Opcode::COMPR:
        reg.compareAndSetSw(reg.getReg(r1), reg.getReg(r2)); break;
    case Opcode::DIVR:
        reg.setReg(r2, reg.getReg(r2) / reg.getReg(r1)); break;
    case Opcode::MULR:
        reg.setReg(r2, reg.getReg(r2) * reg.getReg(r1)); break;
    case Opcode::RMO:
        reg.setReg(r2, reg.getReg(r1)); break;
    // as per instruction sheet lshift is circular!!!
    case Opcode::SHIFTL:
        reg.setReg(r1, (reg.getReg(r1) << (r2 + 1)) | (reg.getReg(r1) >> (24 - r2 - 1))); break;
    case Opcode::SHIFTR:
        reg.setReg(r1, reg.getReg(r1) >> (r2 + 1)); break;
    case Opcode::SUBR:
        reg.setReg(r2, reg.getReg(r2) - reg.getReg(r1)); break;
    case Opcode::SVC:
        notImplemented("SVC"); break;
    case Opcode::TIXR:
        reg.setX(reg.getX() + 1);
        reg.compareAndSetSw(reg.getX(), reg.getReg(r1));
        break;

    default:
        return false;
    }

    return true;
}


int32_t Machine::loadWord(int ni, int operand)
{
    int32_t value;
    if(ni == 1)
        value = operand;
    else if (ni == 2)
        value = getWord(getWord(operand));
    else
        value = getWord(operand);

    return value;
}

uint8_t Machine::loadByte(int ni, int operand)
{
    uint8_t value;
    if(ni == 1)
        value = operand;
    else if (ni == 2)
        value = getByte(getWord(operand));
    else
        value = getByte(operand);
        
    return value;
}

double Machine::loadFloat(int ni, double operand)
{
    double value;
    if(ni == 1)
        value = operand;
    else if (ni == 2)
        value = getFloat(getFloat(operand));
    else
        value = getFloat(operand);
        
    return value;
}

uint32_t Machine::storeAddress(int ni, int operand)
{
    if(ni == 2)
        return getWord(operand);
    return operand;
}


int Machine::execSICF3F4(int opcode, int ni, int operand)
{
    int32_t A = reg.getA();

    switch (opcode)
    {
        case Opcode::ADD: 
            reg.setA(A + loadWord(ni, operand)); break;
        case Opcode::ADDF: 
            reg.setF(reg.getF() + loadFloat(ni, operand)); break;
        case Opcode::AND: 
            reg.setA(A & loadWord(ni, operand)); break;
        case Opcode::COMP: 
            reg.compareAndSetSw(A, loadWord(ni, operand)); break;
        case Opcode::COMPF: 
            reg.compareAndSetSw(reg.getF(), loadFloat(ni, operand)); break;
        case Opcode::DIV: 
            reg.setA(A / loadWord(ni, operand)); break;
        case Opcode::DIVF:
            reg.setF(reg.getF() / loadFloat(ni, operand)); break;

        case Opcode::J:
            // halt detected
            if(reg.getPC() - 3 == storeAddress(ni, operand))
                return HALT;
            reg.setPC(storeAddress(ni, operand)); break;
        case Opcode::JEQ: 
            if(reg.getSW() == Registers::EQ) 
                reg.setPC(storeAddress(ni, operand));
            break;
        case Opcode::JGT: 
            if(reg.getSW() == Registers::GT) 
                reg.setPC(storeAddress(ni, operand));
            break;
        case Opcode::JLT: 
            if(reg.getSW() == Registers::LT) 
                reg.setPC(storeAddress(ni, operand));
            break;
        case Opcode::JSUB:
            reg.setL(reg.getPC());
            reg.setPC(storeAddress(ni, operand));
            break;

        case Opcode::LDA: 
            reg.setA(loadWord(ni, operand)); break;
        case Opcode::LDB: 
            reg.setB(loadWord(ni, operand)); break;
        case Opcode::LDCH: 
            reg.setA(loadByte(ni, operand)); break;
        case Opcode::LDF: 
            reg.setF(loadFloat(ni, operand)); break;
        case Opcode::LDL: 
            reg.setL(loadWord(ni, operand));  break;
        case Opcode::LDS: 
            reg.setS(loadWord(ni, operand));  break;
        case Opcode::LDT: 
            reg.setT(loadWord(ni, operand));  break;
        case Opcode::LDX: 
            reg.setX(loadWord(ni, operand));  break;

        case Opcode::LPS: 
            notImplemented("LPS"); break;
        case Opcode::MUL: 
            reg.setA(A * loadWord(ni, operand)); break;
        case Opcode::MULF:
            reg.setF(reg.getF() * loadFloat(ni, operand)); break;
        case Opcode::OR:
            reg.setA(A | loadWord(ni, operand)); break;
        case Opcode::RD:
            reg.setA(getDevice(loadByte(ni, operand))->read()); break;
        case Opcode::RSUB:
            reg.setPC(reg.getL()); break;
        case Opcode::SSK:
            notImplemented("SSK"); break;

        case Opcode::STA: 
            setWord(storeAddress(ni, operand), A); break;
        case Opcode::STB: 
            setWord(storeAddress(ni, operand), reg.getB());
        case Opcode::STCH:
            setByte(storeAddress(ni, operand), A); break;
        // TODO
        case Opcode::STF: 
            setFloat(storeAddress(ni, operand), reg.getF()); break;
        case Opcode::STI: 
            notImplemented("STI"); break;
        case Opcode::STL:
            setWord(storeAddress(ni, operand), reg.getL()); break;
        case Opcode::STS:
            setWord(storeAddress(ni, operand), reg.getS()); break;
        case Opcode::STSW:
            setWord(storeAddress(ni, operand), reg.getSW()); break;
        case Opcode::STT: 
            setWord(storeAddress(ni, operand), reg.getT()); break;
        case Opcode::STX: 
            setWord(storeAddress(ni, operand), reg.getX()); break;

        case Opcode::SUB: 
            reg.setA(A - loadWord(ni, operand)); break;
        case Opcode::SUBF:
            reg.setF(reg.getF() - loadFloat(ni, operand)); break;
        case Opcode::TD:
            // < ok, = notOk
            reg.compareAndSetSw(-1, (int)getDevice(loadByte(ni, operand))->test()); break;
        case Opcode::TIX:
            reg.setX(reg.getX() + 1);
            reg.compareAndSetSw(reg.getX(), loadWord(ni, operand));
            break;
        case Opcode::WD: 
            getDevice(loadByte(ni, operand))->write(A); break;
        default:
            return 0;
    }

    return 1;
}

uint8_t Machine::fetch()
{
    int32_t pcValue = reg.getPC();
    reg.setPC(pcValue + 1);

    return memory[pcValue];
}

int Machine::execute()
{
    int opcode = fetch();
    // printf("opcode: %X\n", opcode);

    if(execF1(opcode)) 
        return 1;

    int op = fetch();
    // printf("op: %X\n", op);

    if(execF2(opcode, op)) 
        return 1;

    int ni = opcode & 3;
    int x = (op >> 7) & 1;
    int b = (op >> 6) & 1;
    int p = (op >> 5) & 1;
    int e = (op >> 4) & 1;

    opcode &= ~3;

    if(ni == 0)
    {
        // SIC
        op = ((op & 0x7F) << 8) | fetch();
        // printf("sic: %X\n", op);
    }else if(e)
    {
        // F4
        op = ((op & 15) << 16) | (fetch() << 8) | fetch();
        if(b + e) 
            invalidAddressing("Exteended can not be relative");
        // printf("ext: %X\n", op);
    }
    else
    {
        //F3
        op = ((op & 15) << 8) | fetch();

        if(b && !p)
            op += reg.getB();
        else if(!b && p)
        {
            op = op >= 2048 ? op - 4096 : op;
            op += reg.getPC();
        }
        else if(b && p)
            invalidAddressing("Can't use B and P at the same time");

        // printf("F3: %X\n", op);
    }

    if(x)
    {
        // indexed
        if(ni = 3)
            op += reg.getX();
        else
            invalidAddressing("Indexed only supported in simple adressing");
        
        // printf("index: %X\n", op);
    }

    if(int halt = execSICF3F4(opcode, ni, op))
        return halt;

    invalidOpcode(opcode);
    return 0;
}

void Machine::reset() {
    reg.clearRegisters();
    memset(memory, 0, MAX_ADDRESS);
    if(currentFile != "")
    {
        loadSection(currentFile);
    }
}

bool Machine::loadSection(std::string fileName)
{
    currentFile = "";
    reset();

    currentFile = fileName;

    std::fstream r(fileName, std::fstream::in);
    // header
    readString(r, 7);
    int32_t startAddress = readWord(r);
    int codeLength = readWord(r);
    r.get(); // remove newl

    // read text lines until we get to E
    while (r.get() != 'E')
    {
        startAddress = readWord(r);
        int instrCount = readByte(r);
        
        // load bytes from startAddress onwards
        for(auto i = 0; i < instrCount; i++)
        {
            setByte(startAddress + i, readByte(r));
        }
        r.get(); // remove newl
    }
    r.close();
    return true;
}
