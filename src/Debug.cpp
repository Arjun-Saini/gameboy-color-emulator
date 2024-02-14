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
        case 3:
            start = 0x8000;
            end = 0x9FFF;
            break;
        case 4:
            start = 0xFE00;
            end = 0xFE9F;
            break;
        case 5:
            start = 0x0;
            end = 0xFFFF;
            break;
    }

    for(int i = start; i <= end; i++){
        std::cout << std::hex << int(cpu->mmu.read_byte(i)) << " ";
        if(!((i + 1 )% width)){
            std::cout << std::endl;
        }
    }
}

void Debug::print_registers() {
    std::cout << std::endl << "Registers:" << std::endl;
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

    std::cout << std::endl;
}

void Debug::print_info() {
    std::cout << std::endl << "MBC: " << int(cpu->mmu.MBC) << std::endl;
    std::cout << "ROM bank: " << int(cpu->mmu.ROM_bank) << "   RAM bank: " << int(cpu->mmu.RAM_bank) << std::endl;
    std::cout << "Program counter: " << std::hex << cpu->program_counter << std::endl;
    std::cout << "Next bytes: " << std::hex
        << int(cpu->mmu.read_byte(cpu->program_counter)) << " "
        << int(cpu->mmu.read_byte(cpu->program_counter + 1)) << " "
        << int(cpu->mmu.read_byte(cpu->program_counter + 2)) << " "
        << int(cpu->mmu.read_byte(cpu->program_counter + 3)) << std::endl;
    std::cout << "Stack pointer: " << std::hex << cpu->stack_pointer << std::endl;
}