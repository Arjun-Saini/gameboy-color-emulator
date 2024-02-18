#include "Debug.h"

Debug::Debug(CPU *c) {
    cpu = c;
    Log = std::ofstream("log.txt");
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
        std::cout << std::hex << std::setfill('0') << std::setw(2) << int(cpu->mmu.read_byte(i)) << " ";
        if(!((i + 1) % 16)){
//            std::cout << " | ";
        }
        if(!((i + 1) % width)){
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
    std::cout << "IME: " << cpu->IME << std::endl;
    std::cout << "Program counter: " << std::hex << cpu->program_counter << std::endl;
    std::cout << "Next bytes: ";
    for(int i = 0; i < 8; i++){
        std::cout << std::hex << int(cpu->mmu.read_byte(cpu->program_counter + i)) << " ";
    }
    std::cout << std::endl;
    std::cout << "Stack pointer: " << std::hex << cpu->stack_pointer << std::endl;
    std::cout << "Stack bytes centered at pointer: ";
    for(int i = 0; i < 5; i++){
        std::cout << std::hex << int(cpu->mmu.read_byte(cpu->stack_pointer + i - 2)) << " ";
    }
    std::cout << std::endl;
}

void Debug::doctor_log() {
    Log << "A:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.A);
    Log << " F:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.F);
    Log << " B:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.B);
    Log << " C:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.C);
    Log << " D:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.D);
    Log << " E:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.E);
    Log << " H:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.H);
    Log << " L:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->registers.L);
    Log << " SP:" << std::setfill('0') << std::setw(4) << std::right << std::hex << int(cpu->stack_pointer);
    Log << " PC:" << std::setfill('0') << std::setw(4) << std::right << std::hex << int(cpu->program_counter);
    Log << " PCMEM:" << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->mmu.read_byte(cpu->program_counter)) << ",";
    Log << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->mmu.read_byte(cpu->program_counter + 1)) << ",";
    Log << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->mmu.read_byte(cpu->program_counter + 2)) << ",";
    Log << std::setfill('0') << std::setw(2) << std::right << std::hex << int(cpu->mmu.read_byte(cpu->program_counter + 3)) << std::endl;

}