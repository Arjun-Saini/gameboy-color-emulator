#ifndef GAMEBOY_COLOR_EMULATOR_DEBUG_H
#define GAMEBOY_COLOR_EMULATOR_DEBUG_H

#include "CPU.h"
#include <iostream>

class Debug {
public:
    CPU* cpu;
    std::ofstream Log;

    Debug(CPU* cpu);
    void print_gb_mem(int mode, int width);
    void print_registers();
    void print_info();
    void doctor_log();
};


#endif //GAMEBOY_COLOR_EMULATOR_DEBUG_H
