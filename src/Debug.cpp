#include "Debug.h"

void Debug::print_gb_mem(CPU cpu, int mode) {
    int start;
    int end;

    switch (mode) {
        case 0:
            start = 0x0000;
            end = 0x3FFF;
            break;
        case 1:
            start = 0x4000;
            end = 0x7FFF;
            break;
        case 2:
            start = 0xFF80;
            end = 0xFFFE;
            break;
    }

    for(int i = start; i <= end; i++){
        std::cout << std::hex << int(cpu.mmu.gb_memory[i]) << " ";
        if(!((i + 1 )% 32)){
            std::cout << std::endl;
        }
    }
}