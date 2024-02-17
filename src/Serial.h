#ifndef GAMEBOY_COLOR_EMULATOR_SERIAL_H
#define GAMEBOY_COLOR_EMULATOR_SERIAL_H

#include <cstdint>
#include "MMU.h"
#include "CPU.h"

#define SB 0xFF01
#define SC 0xFF02

class Serial {
public:
    uint16_t cycles = 0;
    uint8_t bit_counter = 0;

    MMU* mmu;
    CPU* cpu;

    void tick();
};


#endif //GAMEBOY_COLOR_EMULATOR_SERIAL_H
