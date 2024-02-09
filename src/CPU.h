#include <iostream>
#include "MMU.h"
#include "Registers.h"

#ifndef GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
#define GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H

class CPU {
public:
    typedef int (CPU::*opcode_func)(uint16_t, uint16_t);

    uint16_t program_counter;
    uint16_t stack_pointer;

    Registers registers;
    MMU mmu;

    uint8_t t_cycles;
    uint32_t cycles_per_frame;

    CPU();

    uint16_t next_opcode();
    void decode_opcode(uint16_t opcode);

    void LD_r8_r8(uint8_t* r1, uint8_t r2);
};


#endif //GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
