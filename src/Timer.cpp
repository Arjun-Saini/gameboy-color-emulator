#include "Timer.h"

void Timer::tick() {
    mmu->gb_memory[DIV_addr] = (++DIV & 0xFF00) >> 8;

    uint8_t bit_pos;
    switch (mmu->gb_memory[TAC_addr] & 3) {
        case 0b00:
            bit_pos = 9;
            break;
        case 0b01:
            bit_pos = 3;
            break;
        case 0b10:
            bit_pos = 5;
            break;
        case 0b11:
            bit_pos = 7;
            break;
    }

    uint8_t timer_enable = (mmu->gb_memory[TAC_addr] >> 2) & 1;
    uint8_t result = ((DIV >> bit_pos) & 1) & timer_enable;

    if(prev_result && !result){
        if(mmu->gb_memory[TIMA_addr]++ == 0xFF){
            TMA_reload = true;
        }
    }

    prev_result = result;
}