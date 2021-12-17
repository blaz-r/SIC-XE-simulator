QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ./sim/include

CONFIG += c++14
CONFIG += console

QMAKE_LFLAGS += -static
CONFIG += -static-libgcc -static-libstdc++

#set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    memoryview.cpp \
    sim/machine.cpp \
    sim/device.cpp \
    sim/opcode.cpp \
    sim/reader.cpp \
    sim/registers.cpp \
    sim/runner.cpp

HEADERS += \
    mainwindow.h \
    memoryview.h \
    sim/include/machine.h \
    sim/include/device.h \
    sim/include/opcode.h \
    sim/include/reader.h \
    sim/include/registers.h \
    sim/include/runner.h \
    sim/include/timer.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
