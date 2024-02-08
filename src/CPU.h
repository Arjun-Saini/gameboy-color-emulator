#include <iostream>
#include "MMU.h"
#include "Registers.h"

#ifndef GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
#define GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H

class CPU {
public:
    uint16_t program_counter;
    uint16_t stack_pointer;
    Registers registers;
    MMU mmu;

    uint32_t cycles_per_second;
    uint8_t frames_per_second;
    uint32_t cycles_per_frame;

    uint8_t screen[160][144][3];

    CPU();

    uint16_t next_opcode();
    void decode_opcode(uint16_t opcode);


};


#endif //GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
