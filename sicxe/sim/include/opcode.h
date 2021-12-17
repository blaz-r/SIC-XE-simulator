#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>

class Opcode
{

public:
    Opcode() {}
    ~Opcode() {}

    static const int ADD = 0x18;
    static const int ADDF = 0x58;
    static const int ADDR = 0x90;
    static const int AND = 0x40;
    static const int CLEAR = 0xB4;
    static const int COMP = 0x28;
    static const int COMPF = 0x88;
    static const int COMPR = 0xA0;
    static const int DIV = 0x24;
    static const int DIVF = 0x64;
    static const int DIVR = 0x9C;
    static const int FIX = 0xC4;
    static const int FLOAT = 0xC0;
    static const int HIO = 0xF4;
    static const int J = 0x3C;
    static const int JEQ = 0x30;
    static const int JGT = 0x34;
    static const int JLT = 0x38;
    static const int JSUB = 0x48;
    static const int LDA = 0x00;
    static const int LDB = 0x68;
    static const int LDCH = 0x50;
    static const int LDF = 0x70;
    static const int LDL = 0x08;
    static const int LDS = 0x6C;
    static const int LDT = 0x74;
    static const int LDX = 0x04;
    static const int LPS = 0xD0;
    static const int MUL = 0x20;
    static const int MULF = 0x60;
    static const int MULR = 0x98;
    static const int NORM = 0xC8;
    static const int OR = 0x44;
    static const int RD = 0xD8;
    static const int RMO = 0xAC;
    static const int RSUB = 0x4C;
    static const int SHIFTL = 0xA4;
    static const int SHIFTR = 0xA8;
    static const int SIO = 0xF0;
    static const int SSK = 0xEC;
    static const int STA = 0x0C;
    static const int STB = 0x78;
    static const int STCH = 0x54;
    static const int STF = 0x80;
    static const int STI = 0xD4;
    static const int STL = 0x14;
    static const int STS = 0x7C;
    static const int STSW = 0xE8;
    static const int STT = 0x84;
    static const int STX = 0x10;
    static const int SUB = 0x1C;
    static const int SUBF = 0x5C;
    static const int SUBR = 0x94;
    static const int SVC = 0xB0;
    static const int TD = 0xE0;
    static const int TIO = 0xF8;
    static const int TIX = 0x2C;
    static const int TIXR = 0xB8;
    static const int WD = 0xDC;
};

#endif