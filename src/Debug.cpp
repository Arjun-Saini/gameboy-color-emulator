#include "Debug.h"

Debug::Debug(CPU *c) {
    cpu = c;
}

void Debug::print_gb_mem(int mode, int width) {
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
        std::cout << std::hex << int(cpu->mmu.gb_memory[i]) << " ";
        if(!((i + 1 )% width)){
            std::cout << std::endl;
        }
    }
}

void Debug::print_registers() {
    std::cout << "Registers:" << std::endl;
    std::cout << "A: " << std::hex << int(cpu->registers.A) << " -- ";
    std::cout << "F: " << std::hex << int(cpu->registers.F) << std::endl;
    std::cout << "B: " << std::hex << int(cpu->registers.B) << " -- ";
    std::cout << "C: " << std::hex << int(cpu->registers.C) << std::endl;
    std::cout << "D: " << std::hex << int(cpu->registers.D) << " -- ";
    std::cout << "E: " << std::hex << int(cpu->registers.E) << std::endl;
    std::cout << "H: " << std::hex << int(cpu->registers.H) << " -- ";
    std::cout << "L: " << std::hex << int(cpu->registers.L) << std::endl;

    std::cout << "Flags:" << std::endl;
    std::cout << "Z: " << int(cpu->registers.get_Z()) << " -- ";
    std::cout << "N: " << int(cpu->registers.get_N()) << std::endl;
    std::cout << "H: " << int(cpu->registers.get_H()) << " -- ";
    std::cout << "C: " << int(cpu->registers.get_C()) << std::endl;
}