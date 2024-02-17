#include "Serial.h"

void Serial::tick() {
    cycles++;
    if(cycles == 512){
        cycles = 0;
        if(mmu->read_byte(SC) == 0x81){
            if(bit_counter == 7){
                std::cout << int(mmu->read_byte(SB)) << std::endl;
                mmu->write_byte(SB, 0);
                mmu->write_byte(SC, 1);
                cpu->request_interrupt(3);
            }
            bit_counter++;
        }
    }
}