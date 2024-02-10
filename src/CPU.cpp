#include "CPU.h"

CPU::CPU() {
    mmu = MMU();
    registers = Registers();
    t_cycles = 0;
    cycles_per_frame = 69905;

    // Initialize values
//    program_counter = 0x100;
    stack_pointer = 0xFFFE;
    registers.set_AF(0x01B0);
    registers.set_BC(0x0013);
    registers.set_DE(0x00D8);
    registers.set_HL(0x014D);
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

// Loads value from r2 into r1
void CPU::LD_r8_r8(uint8_t* r1, uint8_t r2) {
    *r1 = r2;
    t_cycles += 4;
}

// Loads the next byte into r
void CPU::LD_r8_u8(uint8_t *r) {
    uint8_t u8 = mmu.read_byte(program_counter++);
    *r = u8;
    t_cycles += 8;
}

// Loads next two bytes into r2 and r1
void CPU::LD_r16_u16(uint8_t *r1, uint8_t *r2) {
    uint8_t u8 = mmu.read_byte(program_counter++);
    *r2 = u8;
    u8 = mmu.read_byte(program_counter++);
    *r1 = u8;
    t_cycles += 12;
}

// Loads value from r into byte pointed to by HL
void CPU::LD_pHL_r8(uint8_t r) {
    mmu.write_byte(registers.get_HL(), r);
    t_cycles += 8;
}

// Loads value from next byte into byte pointed to by HL
void CPU::LD_pHL_n8() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    mmu.write_byte(registers.get_HL(), u8);
    t_cycles += 8;
}

// Loads value from byte pointed to by HL into r
void CPU::LD_r8_pHL(uint8_t *r) {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    *r = pHL;
    t_cycles += 8;
}

// Loads value from A into byte pointed to by r
void CPU::LD_pr16_A(uint16_t r) {
    mmu.write_byte(r, registers.A);
    t_cycles += 8;
}

// Loads value from A into byte pointed to by next two bytes
void CPU::LD_pu16_A() {
    uint16_t u16 = mmu.read_byte(program_counter++);
    u16 |= (mmu.read_byte(program_counter++) << 8);
    mmu.write_byte(u16, registers.A);
    t_cycles += 16;
}

// Loads value from A into byte pointed to by 0xFF00 + next byte
void CPU::LD_pFF00u8_A() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    mmu.write_byte(0xFF00 | u8, registers.A);
    t_cycles += 12;
}

// Loads value from A into byte pointed to by 0xFF00 + C
void CPU::LD_pC_A() {
    mmu.write_byte(0xFF00 | registers.C, registers.A);
    t_cycles += 8;
}

// Loads value from byte pointed to by r into A
void CPU::LD_A_pr16(uint16_t r) {
    registers.A = mmu.read_byte(r);
    t_cycles += 8;
}

void CPU::LD_A_pu16() {
    uint16_t u16 = mmu.read_byte(program_counter++);
    u16 |= (mmu.read_byte(program_counter++) << 8);
    registers.A = mmu.read_byte(u16);
    t_cycles += 16;
}

void CPU::LD_A_pFF00u8() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    registers.A = mmu.read_byte(0xFF00 | u8);
    t_cycles += 12;
}

void CPU::LD_A_pC() {
    registers.A = mmu.read_byte(0xFF00 | registers.C);
    t_cycles += 8;
}

void CPU::LD_pHLI_A() {
    mmu.write_byte(registers.get_HL(), registers.A);
    registers.set_HL(registers.get_HL() + 1);
    t_cycles += 8;
}

void CPU::LD_pHLD_A() {
    mmu.write_byte(registers.get_HL(), registers.A);
    registers.set_HL(registers.get_HL() - 1);
    t_cycles += 8;
}

void CPU::LD_A_pHLD() {
    registers.A = mmu.read_byte(registers.get_HL());
    registers.set_HL(registers.get_HL() - 1);
    t_cycles += 8;
}

void CPU::LD_A_pHLI() {
    registers.A = mmu.read_byte(registers.get_HL());
    registers.set_HL(registers.get_HL() + 1);
    t_cycles += 8;
}

void CPU::LD_SP_u16() {
    uint16_t u16 = mmu.read_byte(program_counter++);
    u16 |= (mmu.read_byte(program_counter++) << 8);
    stack_pointer = u16;
    t_cycles += 12;
}

void CPU::LD_pu16_SP() {
    uint16_t u16 = mmu.read_byte(program_counter++);
    u16 |= (mmu.read_byte(program_counter++) << 8);
    mmu.write_byte(u16, stack_pointer & 0xFF);
    mmu.write_byte(u16 + 1, stack_pointer >> 8);
    t_cycles += 20;
}

void CPU::LD_HL_SPi8() {
    int8_t i8 = mmu.read_byte(program_counter++);
    registers.set_HL(stack_pointer + i8);
    registers.b8_carry(stack_pointer, i8);
    registers.b4_half_carry(stack_pointer, i8);
    registers.set_Z(0);
    registers.set_N(0);
    t_cycles += 12;
}

void CPU::LD_SP_HL() {
    stack_pointer = registers.get_HL();
    t_cycles += 8;
}

// Returns opcode at program_counter
uint16_t CPU::next_opcode() {
    uint8_t opcode = mmu.read_byte(program_counter++);
    if(opcode == 0xCB){
        return (opcode << 8) | next_opcode();
    }
    return opcode;
}

// Decodes opcode and executes instruction
void CPU::decode_opcode(uint16_t opcode) {
    if((opcode & 0xFF00) >> 8 == 0xCB){
        // CB-prefixed opcodes
        switch (opcode & 0xFF) {
            case 0x00:
                break;
            case 0x01:
                break;
            case 0x02:
                break;
            case 0x03:
                break;
            case 0x04:
                break;
            case 0x05:
                break;
            case 0x06:
                break;
            case 0x07:
                break;
            case 0x08:
                break;
            case 0x09:
                break;
            case 0x0A:
                break;
            case 0x0B:
                break;
            case 0x0C:
                break;
            case 0x0D:
                break;
            case 0x0E:
                break;
            case 0x0F:
                break;
            case 0x10:
                break;
            case 0x11:
                break;
            case 0x12:
                break;
            case 0x13:
                break;
            case 0x14:
                break;
            case 0x15:
                break;
            case 0x16:
                break;
            case 0x17:
                break;
            case 0x18:
                break;
            case 0x19:
                break;
            case 0x1A:
                break;
            case 0x1B:
                break;
            case 0x1C:
                break;
            case 0x1D:
                break;
            case 0x1E:
                break;
            case 0x1F:
                break;
            case 0x20:
                break;
            case 0x21:
                break;
            case 0x22:
                break;
            case 0x23:
                break;
            case 0x24:
                break;
            case 0x25:
                break;
            case 0x26:
                break;
            case 0x27:
                break;
            case 0x28:
                break;
            case 0x29:
                break;
            case 0x2A:
                break;
            case 0x2B:
                break;
            case 0x2C:
                break;
            case 0x2D:
                break;
            case 0x2E:
                break;
            case 0x2F:
                break;
            case 0x30:
                break;
            case 0x31:
                break;
            case 0x32:
                break;
            case 0x33:
                break;
            case 0x34:
                break;
            case 0x35:
                break;
            case 0x36:
                break;
            case 0x37:
                break;
            case 0x38:
                break;
            case 0x39:
                break;
            case 0x3A:
                break;
            case 0x3B:
                break;
            case 0x3C:
                break;
            case 0x3D:
                break;
            case 0x3E:
                break;
            case 0x3F:
                break;
            case 0x40:
                break;
            case 0x41:
                break;
            case 0x42:
                break;
            case 0x43:
                break;
            case 0x44:
                break;
            case 0x45:
                break;
            case 0x46:
                break;
            case 0x47:
                break;
            case 0x48:
                break;
            case 0x49:
                break;
            case 0x4A:
                break;
            case 0x4B:
                break;
            case 0x4C:
                break;
            case 0x4D:
                break;
            case 0x4E:
                break;
            case 0x4F:
                break;
            case 0x50:
                break;
            case 0x51:
                break;
            case 0x52:
                break;
            case 0x53:
                break;
            case 0x54:
                break;
            case 0x55:
                break;
            case 0x56:
                break;
            case 0x57:
                break;
            case 0x58:
                break;
            case 0x59:
                break;
            case 0x5A:
                break;
            case 0x5B:
                break;
            case 0x5C:
                break;
            case 0x5D:
                break;
            case 0x5E:
                break;
            case 0x5F:
                break;
            case 0x60:
                break;
            case 0x61:
                break;
            case 0x62:
                break;
            case 0x63:
                break;
            case 0x64:
                break;
            case 0x65:
                break;
            case 0x66:
                break;
            case 0x67:
                break;
            case 0x68:
                break;
            case 0x69:
                break;
            case 0x6A:
                break;
            case 0x6B:
                break;
            case 0x6C:
                break;
            case 0x6D:
                break;
            case 0x6E:
                break;
            case 0x6F:
                break;
            case 0x70:
                break;
            case 0x71:
                break;
            case 0x72:
                break;
            case 0x73:
                break;
            case 0x74:
                break;
            case 0x75:
                break;
            case 0x76:
                break;
            case 0x77:
                break;
            case 0x78:
                break;
            case 0x79:
                break;
            case 0x7A:
                break;
            case 0x7B:
                break;
            case 0x7C:
                break;
            case 0x7D:
                break;
            case 0x7E:
                break;
            case 0x7F:
                break;
            case 0x80:
                break;
            case 0x81:
                break;
            case 0x82:
                break;
            case 0x83:
                break;
            case 0x84:
                break;
            case 0x85:
                break;
            case 0x86:
                break;
            case 0x87:
                break;
            case 0x88:
                break;
            case 0x89:
                break;
            case 0x8A:
                break;
            case 0x8B:
                break;
            case 0x8C:
                break;
            case 0x8D:
                break;
            case 0x8E:
                break;
            case 0x8F:
                break;
            case 0x90:
                break;
            case 0x91:
                break;
            case 0x92:
                break;
            case 0x93:
                break;
            case 0x94:
                break;
            case 0x95:
                break;
            case 0x96:
                break;
            case 0x97:
                break;
            case 0x98:
                break;
            case 0x99:
                break;
            case 0x9A:
                break;
            case 0x9B:
                break;
            case 0x9C:
                break;
            case 0x9D:
                break;
            case 0x9E:
                break;
            case 0x9F:
                break;
            case 0xA0:
                break;
            case 0xA1:
                break;
            case 0xA2:
                break;
            case 0xA3:
                break;
            case 0xA4:
                break;
            case 0xA5:
                break;
            case 0xA6:
                break;
            case 0xA7:
                break;
            case 0xA8:
                break;
            case 0xA9:
                break;
            case 0xAA:
                break;
            case 0xAB:
                break;
            case 0xAC:
                break;
            case 0xAD:
                break;
            case 0xAE:
                break;
            case 0xAF:
                break;
            case 0xB0:
                break;
            case 0xB1:
                break;
            case 0xB2:
                break;
            case 0xB3:
                break;
            case 0xB4:
                break;
            case 0xB5:
                break;
            case 0xB6:
                break;
            case 0xB7:
                break;
            case 0xB8:
                break;
            case 0xB9:
                break;
            case 0xBA:
                break;
            case 0xBB:
                break;
            case 0xBC:
                break;
            case 0xBD:
                break;
            case 0xBE:
                break;
            case 0xBF:
                break;
            case 0xC0:
                break;
            case 0xC1:
                break;
            case 0xC2:
                break;
            case 0xC3:
                break;
            case 0xC4:
                break;
            case 0xC5:
                break;
            case 0xC6:
                break;
            case 0xC7:
                break;
            case 0xC8:
                break;
            case 0xC9:
                break;
            case 0xCA:
                break;
            case 0xCB:
                break;
            case 0xCC:
                break;
            case 0xCD:
                break;
            case 0xCE:
                break;
            case 0xCF:
                break;
            case 0xD0:
                break;
            case 0xD1:
                break;
            case 0xD2:
                break;
            case 0xD3:
                break;
            case 0xD4:
                break;
            case 0xD5:
                break;
            case 0xD6:
                break;
            case 0xD7:
                break;
            case 0xD8:
                break;
            case 0xD9:
                break;
            case 0xDA:
                break;
            case 0xDB:
                break;
            case 0xDC:
                break;
            case 0xDD:
                break;
            case 0xDE:
                break;
            case 0xDF:
                break;
            case 0xE0:
                break;
            case 0xE1:
                break;
            case 0xE2:
                break;
            case 0xE3:
                break;
            case 0xE4:
                break;
            case 0xE5:
                break;
            case 0xE6:
                break;
            case 0xE7:
                break;
            case 0xE8:
                break;
            case 0xE9:
                break;
            case 0xEA:
                break;
            case 0xEB:
                break;
            case 0xEC:
                break;
            case 0xED:
                break;
            case 0xEE:
                break;
            case 0xEF:
                break;
            case 0xF0:
                break;
            case 0xF1:
                break;
            case 0xF2:
                break;
            case 0xF3:
                break;
            case 0xF4:
                break;
            case 0xF5:
                break;
            case 0xF6:
                break;
            case 0xF7:
                break;
            case 0xF8:
                break;
            case 0xF9:
                break;
            case 0xFA:
                break;
            case 0xFB:
                break;
            case 0xFC:
                break;
            case 0xFD:
                break;
            case 0xFE:
                break;
            case 0xFF:
                break;
        }
    }else{
        // Non CB-prefixed opcodes
        switch (opcode & 0xFF) {
            case 0x00:
                break;
            case 0x01:
                LD_r16_u16(&registers.B, &registers.C);
                break;
            case 0x02:
                LD_pr16_A(registers.get_BC());
                break;
            case 0x03:
                break;
            case 0x04:
                break;
            case 0x05:
                break;
            case 0x06:
                LD_r8_u8(&registers.B);
                break;
            case 0x07:
                break;
            case 0x08:
                LD_pu16_SP();
                break;
            case 0x09:
                break;
            case 0x0A:
                LD_A_pr16(registers.get_BC());
                break;
            case 0x0B:
                break;
            case 0x0C:
                break;
            case 0x0D:
                break;
            case 0x0E:
                LD_r8_u8(&registers.C);
                break;
            case 0x0F:
                break;
            case 0x10:
                break;
            case 0x11:
                LD_r16_u16(&registers.D, &registers.E);
                break;
            case 0x12:
                LD_pr16_A(registers.get_DE());
                break;
            case 0x13:
                break;
            case 0x14:
                break;
            case 0x15:
                break;
            case 0x16:
                LD_r8_u8(&registers.D);
                break;
            case 0x17:
                break;
            case 0x18:
                break;
            case 0x19:
                break;
            case 0x1A:
                LD_A_pr16(registers.get_DE());
                break;
            case 0x1B:
                break;
            case 0x1C:
                break;
            case 0x1D:
                break;
            case 0x1E:
                LD_r8_u8(&registers.E);
                break;
            case 0x1F:
                break;
            case 0x20:
                break;
            case 0x21:
                LD_r16_u16(&registers.H, &registers.L);
                break;
            case 0x22:
                LD_pHLI_A();
                break;
            case 0x23:
                break;
            case 0x24:
                break;
            case 0x25:
                break;
            case 0x26:
                LD_r8_u8(&registers.H);
                break;
            case 0x27:
                break;
            case 0x28:
                break;
            case 0x29:
                break;
            case 0x2A:
                LD_A_pHLI();
                break;
            case 0x2B:
                break;
            case 0x2C:
                break;
            case 0x2D:
                break;
            case 0x2E:
                LD_r8_u8(&registers.L);
                break;
            case 0x2F:
                break;
            case 0x30:
                break;
            case 0x31:
                LD_SP_u16();
                break;
            case 0x32:
                LD_pHLD_A();
                break;
            case 0x33:
                break;
            case 0x34:
                break;
            case 0x35:
                break;
            case 0x36:
                LD_pHL_n8();
                break;
            case 0x37:
                break;
            case 0x38:
                break;
            case 0x39:
                break;
            case 0x3A:
                LD_A_pHLD();
                break;
            case 0x3B:
                break;
            case 0x3C:
                break;
            case 0x3D:
                break;
            case 0x3E:
                LD_r8_u8(&registers.A);
                break;
            case 0x3F:
                break;
            case 0x40:
                LD_r8_r8(&registers.B, registers.B);
                break;
            case 0x41:
                LD_r8_r8(&registers.C, registers.B);
                break;
            case 0x42:
                LD_r8_r8(&registers.D, registers.B);
                break;
            case 0x43:
                LD_r8_r8(&registers.E, registers.B);
                break;
            case 0x44:
                LD_r8_r8(&registers.H, registers.B);
                break;
            case 0x45:
                LD_r8_r8(&registers.L, registers.B);
                break;
            case 0x46:
                LD_r8_pHL(&registers.B);
                break;
            case 0x47:
                LD_r8_r8(&registers.B, registers.A);
                break;
            case 0x48:
                LD_r8_r8(&registers.C, registers.B);
                break;
            case 0x49:
                LD_r8_r8(&registers.C, registers.C);
                break;
            case 0x4A:
                LD_r8_r8(&registers.C, registers.D);
                break;
            case 0x4B:
                LD_r8_r8(&registers.C, registers.E);
                break;
            case 0x4C:
                LD_r8_r8(&registers.C, registers.H);
                break;
            case 0x4D:
                LD_r8_r8(&registers.C, registers.L);
                break;
            case 0x4E:
                LD_r8_pHL(&registers.C);
                break;
            case 0x4F:
                LD_r8_r8(&registers.C, registers.A);
                break;
            case 0x50:
                LD_r8_r8(&registers.D, registers.B);
                break;
            case 0x51:
                LD_r8_r8(&registers.D, registers.C);
                break;
            case 0x52:
                LD_r8_r8(&registers.D, registers.D);
                break;
            case 0x53:
                LD_r8_r8(&registers.D, registers.E);
                break;
            case 0x54:
                LD_r8_r8(&registers.D, registers.H);
                break;
            case 0x55:
                LD_r8_r8(&registers.D, registers.L);
                break;
            case 0x56:
                LD_r8_pHL(&registers.D);
                break;
            case 0x57:
                LD_r8_r8(&registers.D, registers.A);
                break;
            case 0x58:
                LD_r8_r8(&registers.E, registers.B);
                break;
            case 0x59:
                LD_r8_r8(&registers.E, registers.C);
                break;
            case 0x5A:
                LD_r8_r8(&registers.E, registers.D);
                break;
            case 0x5B:
                LD_r8_r8(&registers.E, registers.E);
                break;
            case 0x5C:
                LD_r8_r8(&registers.E, registers.H);
                break;
            case 0x5D:
                LD_r8_r8(&registers.E, registers.L);
                break;
            case 0x5E:
                LD_r8_pHL(&registers.E);
                break;
            case 0x5F:
                LD_r8_r8(&registers.E, registers.A);
                break;
            case 0x60:
                LD_r8_r8(&registers.H, registers.B);
                break;
            case 0x61:
                LD_r8_r8(&registers.H, registers.C);
                break;
            case 0x62:
                LD_r8_r8(&registers.H, registers.D);
                break;
            case 0x63:
                LD_r8_r8(&registers.H, registers.E);
                break;
            case 0x64:
                LD_r8_r8(&registers.H, registers.H);
                break;
            case 0x65:
                LD_r8_r8(&registers.H, registers.L);
                break;
            case 0x66:
                LD_r8_pHL(&registers.H);
                break;
            case 0x67:
                LD_r8_r8(&registers.H, registers.A);
                break;
            case 0x68:
                LD_r8_r8(&registers.L, registers.B);
                break;
            case 0x69:
                LD_r8_r8(&registers.L, registers.C);
                break;
            case 0x6A:
                LD_r8_r8(&registers.L, registers.D);
                break;
            case 0x6B:
                LD_r8_r8(&registers.L, registers.E);
                break;
            case 0x6C:
                LD_r8_r8(&registers.L, registers.H);
                break;
            case 0x6D:
                LD_r8_r8(&registers.L, registers.L);
                break;
            case 0x6E:
                LD_r8_pHL(&registers.L);
                break;
            case 0x6F:
                LD_r8_r8(&registers.L, registers.A);
                break;
            case 0x70:
                LD_pHL_r8(registers.B);
                break;
            case 0x71:
                LD_pHL_r8(registers.C);
                break;
            case 0x72:
                LD_pHL_r8(registers.D);
                break;
            case 0x73:
                LD_pHL_r8(registers.E);
                break;
            case 0x74:
                LD_pHL_r8(registers.H);
                break;
            case 0x75:
                LD_pHL_r8(registers.L);
                break;
            case 0x76:
                break;
            case 0x77:
                LD_pHL_r8(registers.A);
                break;
            case 0x78:
                LD_r8_r8(&registers.A, registers.B);
                break;
            case 0x79:
                LD_r8_r8(&registers.A, registers.C);
                break;
            case 0x7A:
                LD_r8_r8(&registers.A, registers.D);
                break;
            case 0x7B:
                LD_r8_r8(&registers.A, registers.E);
                break;
            case 0x7C:
                LD_r8_r8(&registers.A, registers.H);
                break;
            case 0x7D:
                LD_r8_r8(&registers.A, registers.L);
                break;
            case 0x7E:
                LD_r8_pHL(&registers.A);
                break;
            case 0x7F:
                LD_r8_r8(&registers.A, registers.A);
                break;
            case 0x80:
                break;
            case 0x81:
                break;
            case 0x82:
                break;
            case 0x83:
                break;
            case 0x84:
                break;
            case 0x85:
                break;
            case 0x86:
                break;
            case 0x87:
                break;
            case 0x88:
                break;
            case 0x89:
                break;
            case 0x8A:
                break;
            case 0x8B:
                break;
            case 0x8C:
                break;
            case 0x8D:
                break;
            case 0x8E:
                break;
            case 0x8F:
                break;
            case 0x90:
                break;
            case 0x91:
                break;
            case 0x92:
                break;
            case 0x93:
                break;
            case 0x94:
                break;
            case 0x95:
                break;
            case 0x96:
                break;
            case 0x97:
                break;
            case 0x98:
                break;
            case 0x99:
                break;
            case 0x9A:
                break;
            case 0x9B:
                break;
            case 0x9C:
                break;
            case 0x9D:
                break;
            case 0x9E:
                break;
            case 0x9F:
                break;
            case 0xA0:
                break;
            case 0xA1:
                break;
            case 0xA2:
                break;
            case 0xA3:
                break;
            case 0xA4:
                break;
            case 0xA5:
                break;
            case 0xA6:
                break;
            case 0xA7:
                break;
            case 0xA8:
                break;
            case 0xA9:
                break;
            case 0xAA:
                break;
            case 0xAB:
                break;
            case 0xAC:
                break;
            case 0xAD:
                break;
            case 0xAE:
                break;
            case 0xAF:
                break;
            case 0xB0:
                break;
            case 0xB1:
                break;
            case 0xB2:
                break;
            case 0xB3:
                break;
            case 0xB4:
                break;
            case 0xB5:
                break;
            case 0xB6:
                break;
            case 0xB7:
                break;
            case 0xB8:
                break;
            case 0xB9:
                break;
            case 0xBA:
                break;
            case 0xBB:
                break;
            case 0xBC:
                break;
            case 0xBD:
                break;
            case 0xBE:
                break;
            case 0xBF:
                break;
            case 0xC0:
                break;
            case 0xC1:
                break;
            case 0xC2:
                break;
            case 0xC3:
                break;
            case 0xC4:
                break;
            case 0xC5:
                break;
            case 0xC6:
                break;
            case 0xC7:
                break;
            case 0xC8:
                break;
            case 0xC9:
                break;
            case 0xCA:
                break;
            case 0xCB:
                break;
            case 0xCC:
                break;
            case 0xCD:
                break;
            case 0xCE:
                break;
            case 0xCF:
                break;
            case 0xD0:
                break;
            case 0xD1:
                break;
            case 0xD2:
                break;
            case 0xD3:
                // UNUSED
                break;
            case 0xD4:
                break;
            case 0xD5:
                break;
            case 0xD6:
                break;
            case 0xD7:
                break;
            case 0xD8:
                break;
            case 0xD9:
                break;
            case 0xDA:
                break;
            case 0xDB:
                // UNUSED
                break;
            case 0xDC:
                break;
            case 0xDD:
                // UNUSED
                break;
            case 0xDE:
                break;
            case 0xDF:
                break;
            case 0xE0:
                LD_pFF00u8_A();
                break;
            case 0xE1:
                break;
            case 0xE2:
                LD_pC_A();
                break;
            case 0xE3:
                // UNUSED
                break;
            case 0xE4:
                // UNUSED
                break;
            case 0xE5:
                break;
            case 0xE6:
                break;
            case 0xE7:
                break;
            case 0xE8:
                break;
            case 0xE9:
                break;
            case 0xEA:
                LD_pu16_A();
                break;
            case 0xEB:
                // UNUSED
                break;
            case 0xEC:
                // UNUSED
                break;
            case 0xED:
                // UNUSED
                break;
            case 0xEE:
                break;
            case 0xEF:
                break;
            case 0xF0:
                LD_A_pFF00u8();
                break;
            case 0xF1:
                break;
            case 0xF2:
                LD_A_pC();
                break;
            case 0xF3:
                break;
            case 0xF4:
                // UNUSED
                break;
            case 0xF5:
                break;
            case 0xF6:
                break;
            case 0xF7:
                break;
            case 0xF8:
                LD_HL_SPi8();
                break;
            case 0xF9:
                LD_SP_HL();
                break;
            case 0xFA:
                LD_A_pu16();
                break;
            case 0xFB:
                break;
            case 0xFC:
                // UNUSED
                break;
            case 0xFD:
                // UNUSED
                break;
            case 0xFE:
                break;
            case 0xFF:
                break;
        }
    }
}