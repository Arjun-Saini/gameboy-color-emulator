#include "CPU.h"

CPU::CPU() {
    mmu = MMU();
    registers = Registers();
    cycles_per_second = 4194304;
    frames_per_second = 60;
    cycles_per_frame = cycles_per_second / frames_per_second;

    // Initialize values
    program_counter = 0x100;
    stack_pointer = 0xFFFE;
    registers.set_reg_AF(0x01B0);
    registers.set_reg_BC(0x0013);
    registers.set_reg_DE(0x00D8);
    registers.set_reg_HL(0x014D);
    mmu.gb_memory[0xFF05] = 0x00;
    mmu.gb_memory[0xFF06] = 0x00;
    mmu.gb_memory[0xFF07] = 0x00;
    mmu.gb_memory[0xFF10] = 0x80;
    mmu.gb_memory[0xFF11] = 0xBF;
    mmu.gb_memory[0xFF12] = 0xF3;
    mmu.gb_memory[0xFF14] = 0xBF;
    mmu.gb_memory[0xFF16] = 0x3F;
    mmu.gb_memory[0xFF17] = 0x00;
    mmu.gb_memory[0xFF19] = 0xBF;
    mmu.gb_memory[0xFF1A] = 0x7F;
    mmu.gb_memory[0xFF1B] = 0xFF;
    mmu.gb_memory[0xFF1C] = 0x9F;
    mmu.gb_memory[0xFF1E] = 0xBF;
    mmu.gb_memory[0xFF20] = 0xFF;
    mmu.gb_memory[0xFF21] = 0x00;
    mmu.gb_memory[0xFF22] = 0x00;
    mmu.gb_memory[0xFF23] = 0xBF;
    mmu.gb_memory[0xFF24] = 0x77;
    mmu.gb_memory[0xFF25] = 0xF3;
    mmu.gb_memory[0xFF26] = 0xF1;
    mmu.gb_memory[0xFF40] = 0x91;
    mmu.gb_memory[0xFF42] = 0x00;
    mmu.gb_memory[0xFF43] = 0x00;
    mmu.gb_memory[0xFF45] = 0x00;
    mmu.gb_memory[0xFF47] = 0xFC;
    mmu.gb_memory[0xFF48] = 0xFF;
    mmu.gb_memory[0xFF49] = 0xFF;
    mmu.gb_memory[0xFF4A] = 0x00;
    mmu.gb_memory[0xFF4B] = 0x00;
    mmu.gb_memory[0xFFFF] = 0x00;
}

// Returns opcode at program_counter
uint16_t CPU::next_opcode() {
    uint8_t opcode = mmu.gb_memory[program_counter++];
    if(opcode == 0xCB){
        return (opcode << 8) | next_opcode();
    }
    return opcode;
}

void CPU::decode_opcode(uint16_t opcode) {
    if((opcode & 0xFF00) >> 2 == 0xCB){
        switch (opcode & 0xFF) {
            // TODO CB opcodes
        }
    }else{
        // TODO Normal opcodes
    }
}