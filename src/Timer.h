#ifndef GAMEBOY_COLOR_EMULATOR_TIMER_H
#define GAMEBOY_COLOR_EMULATOR_TIMER_H

#include <cstdint>
#include "MMU.h"

#define DIV_addr 0xFF04
#define TIMA_addr 0xFF05
#define TMA_addr 0xFF06
#define TAC_addr 0xFF07

class Timer {
public:
    MMU* mmu;

    uint16_t DIV = 0;
    uint8_t prev_result = 0;

    bool TMA_reload = false;

    void tick();
};


#endif //GAMEBOY_COLOR_EMULATOR_TIMER_H
