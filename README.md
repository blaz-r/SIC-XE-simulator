# SIC-XE-simulator

SIC/XE simulator written in C++ with GUI made in QT

![gui](https://github.com/blaz-r/SIC-XE-simulator/blob/main/simulator_gui.jpg)

This project is part of assignment for System Software course on FRI, UNI-LJ.
It was my first time working with QT in C++ so it's by no means perfect, there are some bugs, but overall it should work fine.

## Basic usage:
- load program by clicking **Program** in top menu, then selecting **Load Obj**
- start program by clicking **Run**
- you can step instructions using **Step**
- you can stop the program using **Stop**
- you can reset entire simulator with **Reset** (this resets memory back to it's state before execution, sets all registers to 0 and clears write segment)
- you can adjust execution speed by setting speed in **Speed**, it can work up to about 1MHz
- simulator detects **halt**, when it encounters jump on same row from where it was called (HALT    J   HALT)

There are some test programs included in [obj folder](https://github.com/blaz-r/SIC-XE-simulator/tree/main/obj_files).
- print prints "SIC/XE" to device 11 (11.dev)
- echo writes some text and numbers to console
- float does some operations with float (loaded number should be 420.69)
- arith performs some arithmetic operations (sum, diff, prod, quot, mod)

## Registers:
- register values are shown for each register and are in hexadecimal
- float also has decimal window for easier representation

## Writes:
- every write (byte, word, float) is logged to this window
- it is in form: (**hex address**) -> **hex value** | **decimal value** | ascii{ **ascii value** }

## Memory view
- leftmost column contains addresses of memory of form **{start address of row}**-**{end address of row}**
- addresses are inclusive so **00000-0000F** means first value in view is value on address 00000 and the last is on address 0000F
- middle column contains values at memory addresses in hexadecimal
- when you click on value it is highlighted and it's address is displayed in **Selected address**
- last column contains ascii representation of values at memory location

## Console
- simulator also uses console, you can disable that in qmake file
- devices 0, 1 and 2 are mapped to stdin, stdout, stderr of console
- there are some problems with fonts I think so on start QT prints some errors but it works fine

## Loading
- simulator has an absolute loader, so it doesn't support modification records (M...)
- it reads obj file that is produced by standard SIC/XE assembly

## Float operations
Floating point operations are supported to some extent, but they may not work on every system due to endianness of bits in double.
Problem occurs due to SIC/XE float being 48 bit so last 16 bits are cut off. It detects little and big endian, but double can also be orientated differently in each of those endians.
The code that is in effect now works on my Windows PC compiled with mingw, there's also some other code commented that worked on my Ubuntu with g++ and no qt.
Maybe there's a nice way to make it paltform independent, if you know how, I'd be glad to see it :).
