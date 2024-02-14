#ifndef GAMEBOY_COLOR_EMULATOR_DEBUG_H
#define GAMEBOY_COLOR_EMULATOR_DEBUG_H

#include "CPU.h"

class Debug {
public:
    CPU* cpu;

    Debug(CPU* cpu);
    void print_gb_mem(int mode, int width);
    void print_registers();
    void print_info();
};


#endif //GAMEBOY_COLOR_EMULATOR_DEBUG_H
