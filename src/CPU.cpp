#include "CPU.h"

CPU::CPU() {
    mmu = MMU();
    registers = Registers();
    t_cycles = 0;
    cycles_per_frame = 69905;
    IME = false;
    halted = false;
    skip_handler = false;
    halt_bug = true;

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

void CPU::ADC_A_r8(uint8_t r) {
    registers.b8_carry(registers.A, r, registers.get_C());
    registers.b4_half_carry(registers.A, r, registers.get_C());
    registers.set_N(0);
    registers.A += r + registers.get_C();
    registers.set_Z(registers.A == 0);
    t_cycles += 4;
}

void CPU::ADC_A_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.b8_carry(registers.A, pHL, registers.get_C());
    registers.b4_half_carry(registers.A, pHL, registers.get_C());
    registers.set_N(0);
    registers.A += pHL + registers.get_C();
    registers.set_Z(registers.A == 0);
    t_cycles += 8;
}

void CPU::ADC_A_u8() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    registers.b8_carry(registers.A, u8, registers.get_C());
    registers.b4_half_carry(registers.A, u8, registers.get_C());
    registers.set_N(0);
    registers.A += u8 + registers.get_C();
    registers.set_Z(registers.A == 0);
    t_cycles += 8;
}

void CPU::ADD_A_r8(uint8_t r) {
    registers.b8_carry(registers.A, r);
    registers.b4_half_carry(registers.A, r);
    registers.set_N(0);
    registers.A += + r;
    registers.set_Z(registers.A == 0);
    t_cycles += 4;
}

void CPU::ADD_A_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.b8_carry(registers.A, pHL);
    registers.b4_half_carry(registers.A, pHL);
    registers.set_N(0);
    registers.A += pHL;
    registers.set_Z(registers.A == 0);
    t_cycles += 8;
}

void CPU::ADD_A_u8() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    registers.b8_carry(registers.A, u8);
    registers.b4_half_carry(registers.A, u8);
    registers.set_N(0);
    registers.A += u8;
    registers.set_Z(registers.A == 0);
    t_cycles += 8;
}

void CPU::ADD_HL_r16(uint16_t r) {
    registers.b16_carry(registers.get_HL(), r);
    registers.b12_half_carry(registers.get_HL(), r);
    registers.set_N(0);
    registers.set_HL(registers.get_HL() + r);
    t_cycles += 8;
}

void CPU::ADD_SP_i8() {
    int8_t i8 = mmu.read_byte(program_counter++);
    registers.b8_carry(stack_pointer, i8);
    registers.b4_half_carry(stack_pointer, i8);
    registers.set_Z(0);
    registers.set_N(0);
    stack_pointer += i8;
    t_cycles += 16;
}

void CPU::AND_A_r8(uint8_t r) {
    registers.A &= r;
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(1);
    registers.set_C(0);
    t_cycles += 4;
}

void CPU::AND_A_pHL() {
    registers.A &= mmu.read_byte(registers.get_HL());
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(1);
    registers.set_C(0);
    t_cycles += 8;
}

void CPU::AND_A_u8() {
    registers.A &= mmu.read_byte(program_counter++);
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(1);
    registers.set_C(0);
    t_cycles += 8;
}

void CPU::CP_A_r8(uint8_t r) {
    registers.set_N(1);
    registers.b8_carry(registers.A, -r);
    registers.b4_half_carry(registers.A, -r);
    registers.set_Z(registers.A - r == 0);
    t_cycles += 4;
}

void CPU::CP_A_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.set_N(1);
    registers.b8_carry(registers.A, -pHL);
    registers.b4_half_carry(registers.A, -pHL);
    registers.set_Z(registers.A - pHL == 0);
    t_cycles += 8;
}

void CPU::CP_A_u8() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    registers.set_N(1);
    registers.b8_carry(registers.A, -u8);
    registers.b4_half_carry(registers.A, -u8);
    registers.set_Z(registers.A - u8 == 0);
    t_cycles += 8;
}

void CPU::DEC_r8(uint8_t* r) {
    registers.b4_half_carry(*r, *r - 1);
    (*r)--;
    registers.set_Z(*r == 0);
    registers.set_N(1);
    t_cycles += 4;
}

void CPU::DEC_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.b4_half_carry(pHL, pHL - 1);
    registers.set_HL(--pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(1);
    t_cycles += 12;
}

void CPU::DEC_SP() {
    stack_pointer--;
    t_cycles += 8;
}

void CPU::DEC_r16(uint8_t* r1, uint8_t* r2) {
    uint16_t r16 = (*r1 << 8) | *r2;
    r16--;
    *r1 = r16 & 0xFF00;
    *r2 = r16 & 0xFF;
    t_cycles += 8;
}

void CPU::INC_r8(uint8_t* r) {
    registers.b4_half_carry(*r, *r + 1);
    (*r)++;
    registers.set_Z(*r == 0);
    registers.set_N(0);
    t_cycles += 4;
}

void CPU::INC_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.b4_half_carry(pHL, pHL + 1);
    registers.set_HL(++pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    t_cycles += 12;
}

void CPU::INC_SP() {
    stack_pointer++;
    t_cycles += 8;
}

void CPU::INC_r16(uint8_t* r1, uint8_t* r2) {
    uint16_t r16 = (*r1 << 8) | *r2;
    r16++;
    *r1 = r16 & 0xFF00;
    *r2 = r16 & 0xFF;
    t_cycles += 8;
}

void CPU::OR_A_r8(uint8_t r) {
    registers.A |= r;
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 4;
}

void CPU::OR_A_pHL() {
    registers.A |= mmu.read_byte(registers.get_HL());
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 8;
}

void CPU::OR_A_u8() {
    registers.A |= mmu.read_byte(program_counter++);
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 8;
}

void CPU::SBC_A_r8(uint8_t r) {
    registers.b8_carry(registers.A, -r, -registers.get_C());
    registers.b4_half_carry(registers.A, -r, -registers.get_C());
    registers.A -= r + registers.get_C();
    registers.set_Z(registers.A == 0);
    registers.set_N(1);
    t_cycles += 4;
}

void CPU::SBC_A_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.b8_carry(registers.A, -pHL, -registers.get_C());
    registers.b4_half_carry(registers.A, -pHL, -registers.get_C());
    registers.A -= pHL + registers.get_C();
    registers.set_Z(registers.A == 0);
    registers.set_N(1);
    t_cycles += 8;
}

void CPU::SBC_A_u8() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    registers.b8_carry(registers.A, -u8, -registers.get_C());
    registers.b4_half_carry(registers.A, -u8, -registers.get_C());
    registers.A -= u8 + registers.get_C();
    registers.set_Z(registers.A == 0);
    registers.set_N(1);
    t_cycles += 8;
}

void CPU::SUB_A_r8(uint8_t r) {
    registers.b8_carry(registers.A, -r);
    registers.b4_half_carry(registers.A, -r);
    registers.A -= r;
    registers.set_Z(registers.A == 0);
    registers.set_N(1);
    t_cycles += 4;
}

void CPU::SUB_A_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.b8_carry(registers.A, -pHL);
    registers.b4_half_carry(registers.A, -pHL);
    registers.A -= pHL;
    registers.set_Z(registers.A == 0);
    registers.set_N(1);
    t_cycles += 8;
}

void CPU::SUB_A_u8() {
    uint8_t u8 = mmu.read_byte(program_counter++);
    registers.b8_carry(registers.A, -u8);
    registers.b4_half_carry(registers.A, -u8);
    registers.A -= u8;
    registers.set_Z(registers.A == 0);
    registers.set_N(1);
    t_cycles += 8;
}

void CPU::XOR_A_r8(uint8_t r) {
    registers.A ^= r;
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 4;
}

void CPU::XOR_A_pHL() {
    registers.A ^= mmu.read_byte(registers.get_HL());
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 8;
}

void CPU::XOR_A_u8() {
    registers.A ^= mmu.read_byte(program_counter++);
    registers.set_Z(registers.A == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 8;
}

void CPU::JP_HL() {
    program_counter = registers.get_HL();
    t_cycles += 4;
}

void CPU::JP_u16() {
    uint16_t u16 = mmu.read_byte(program_counter++);
    u16 |= (mmu.read_byte(program_counter++) << 8);
    program_counter = u16;
    t_cycles += 16;
}

void CPU::JP_cc_u16(bool cc) {
    if(cc){
        JP_u16();
    }else{
        program_counter += 2;
        t_cycles += 12;
    }
}

void CPU::CALL_u16() {
    uint16_t next_adr = program_counter + 2;
    mmu.write_byte(--stack_pointer, next_adr & 0xFF00);
    mmu.write_byte(--stack_pointer, next_adr & 0xFF);
    t_cycles += 8;
    JP_u16();
}

void CPU::CALL_cc_u16(bool cc) {
    if(cc){
        CALL_u16();
    }else{
        program_counter += 2;
        t_cycles += 12;
    }
}

void CPU::JR_i8() {
    int8_t i8 = mmu.read_byte(program_counter);
    program_counter += i8;
    t_cycles += 12;
}

void CPU::JR_cc_i8(bool cc) {
    if(cc){
        JR_i8();
    }else{
        program_counter++;
        t_cycles += 8;
    }
}

void CPU::RET() {
    uint16_t addr = mmu.read_byte(stack_pointer++);
    addr |= (mmu.read_byte(stack_pointer++) << 2);
    program_counter = addr;
    t_cycles += 16;
}

void CPU::RET_cc(bool cc) {
    if(cc){
        t_cycles += 4;
        RET();
    }else{
        t_cycles += 8;
    }
}

void CPU::RETI() {
    IME = true;
    RET();
}

void CPU::RST_vec(uint8_t v) {
    uint16_t next_adr = program_counter + 2;
    mmu.write_byte(--stack_pointer, next_adr & 0xFF00);
    mmu.write_byte(--stack_pointer, next_adr & 0xFF);
    program_counter = v;
    t_cycles += 16;
}

void CPU::POP_AF() {
    uint8_t r = mmu.read_byte(stack_pointer++);
    registers.set_Z(r >> 7);
    registers.set_N((r >> 6) & 1);
    registers.set_H((r >> 5) & 1);
    registers.set_C((r >> 4) & 1);
    registers.A = mmu.read_byte(stack_pointer++);
    t_cycles += 12;
}

void CPU::POP_r16(uint8_t* r1, uint8_t* r2) {
    *r2 = mmu.read_byte(stack_pointer++);
    *r1 = mmu.read_byte(stack_pointer++);
    t_cycles += 12;
}

void CPU::PUSH_AF() {
    mmu.write_byte(--stack_pointer, registers.A);
    uint8_t f = registers.get_Z() << 7 | registers.get_N() << 6 | registers.get_H() << 5 | registers.get_C() << 4;
    mmu.write_byte(--stack_pointer, f);
    t_cycles += 16;
}

void CPU::PUSH_r16(uint16_t r) {
    mmu.write_byte(--stack_pointer, r & 0xFF00);
    mmu.write_byte(--stack_pointer, r & 0xFF);
    t_cycles += 16;
}

void CPU::CCF() {
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(~registers.get_C());
    t_cycles += 4;
}

void CPU::CPL() {
    registers.A = ~registers.A;
    registers.set_N(1);
    registers.set_H(1);
    t_cycles += 4;
}

void CPU::DAA() {
    uint8_t adjust = 0;

    // If addition and ones digit > 9, or half carry, adjust by 0x06
    if((!registers.get_N() && (registers.A & 0xF) > 0x09) || registers.get_H()){
        adjust += 0x06;
    }

    // If addition and tens digits > 9, or carry, adjust by 0x60
    if((!registers.get_N() && (registers.A & 0xFF) > 0x99) || registers.get_C()){
        adjust += 0x60;
        registers.set_C(1);
    }else{
        registers.set_C(0);
    }

    if(registers.get_N()){
        registers.A -= adjust;
    }else{
        registers.A += adjust;
    }

    registers.set_Z(registers.A == 0);
    registers.set_H(0);
    t_cycles += 4;
}

void CPU::DI() {
    IME = false;
    t_cycles += 4;
}

void CPU::EI() {
    enable_interrupt = true;
    t_cycles += 4;
}

void CPU::HALT() {
    if(IME){
        // wait for interrupt, then call handler normally
        halted = true;
    }else{
        uint8_t IE = mmu.read_byte(0xFFFF);
        uint8_t IF = mmu.read_byte(0xFF0F);
        if((IE & IF) > 0){
            // continue, cause bug where program counter is not incremented on the next instruction
            halt_bug = true;
        }else{
            // wait for interrupt, but handler is not called
            halted = true;
            skip_handler = true;
        }
    }
    // TODO not sure if this is necessary
//    t_cycles += 4;
}

void CPU::NOP() {
    t_cycles += 4;
}

void CPU::SCF() {
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(1);
    t_cycles += 4;
}

void CPU::STOP() {
    stopped = true;
}

void CPU::BIT_u3_r8(uint8_t u, uint8_t r) {
    registers.set_Z(~((r >> u) & 1));
    registers.set_N(0);
    registers.set_H(1);
    t_cycles += 8;
}

void CPU::BIT_u3_pHL(uint8_t u) {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    registers.set_Z(~((pHL >> u) & 1));
    registers.set_N(0);
    registers.set_H(1);
    t_cycles += 12;
}

void CPU::RES_u3_r8(uint8_t u, uint8_t *r) {
    *r &= ~(1 << u);
    t_cycles += 8;
}

void CPU::RES_u3_pHL(uint8_t u) {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    pHL &= ~(1 << u);
    mmu.write_byte(registers.get_HL(), pHL);
    t_cycles += 16;
}

void CPU::SET_u3_r8(uint8_t u, uint8_t *r) {
    *r |= 1 << u;
    t_cycles += 8;
}

void CPU::SET_u3_pHL(uint8_t u) {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    pHL |= 1 << u;
    mmu.write_byte(registers.get_HL(), pHL);
    t_cycles += 16;
}

void CPU::SWAP_r8(uint8_t *r) {
    uint8_t lower = *r << 4;
    uint8_t upper = *r >> 4;
    *r = lower | upper;
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 8;
}

void CPU::SWAP_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t lower = pHL << 4;
    uint8_t upper = pHL >> 4;
    pHL = lower | upper;
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(0);
    t_cycles += 16;
}

void CPU::RL_r8(uint8_t *r) {
    uint8_t b7 = (*r & 0x80) >> 7;
    *r = (*r << 1) | registers.get_C();
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 8;
}

void CPU::RL_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t b7 = (pHL & 0x80) >> 7;
    pHL = (pHL << 1) | registers.get_C();
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 16;
}

void CPU::RLA() {
    uint8_t b7 = (registers.A & 0x80) >> 7;
    registers.A = (registers.A << 1) | registers.get_C();
    registers.set_Z(0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 4;
}

void CPU::RLC_r8(uint8_t *r) {
    uint8_t b7 = (*r & 0x80) >> 7;
    *r = (*r << 1) | b7;
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 8;
}

void CPU::RLC_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t b7 = (pHL & 0x80) >> 7;
    pHL = (pHL << 1) | b7;
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 16;
}

void CPU::RLCA() {
    uint8_t b7 = (registers.A & 0x80) >> 7;
    registers.A = (registers.A << 1) | b7;
    registers.set_Z(0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 4;
}

void CPU::RR_r8(uint8_t *r) {
    uint8_t b7 = *r & 1;
    *r = (registers.get_C() << 7) | (*r >> 1);
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 8;
}

void CPU::RR_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t b7 = pHL & 1;
    pHL = (registers.get_C() << 7) | (pHL >> 1);
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 16;
}

void CPU::RRA() {
    uint8_t b7 = registers.A & 1;
    registers.A = (registers.get_C() << 7) | (registers.A >> 1);
    registers.set_Z(0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 4;
}

void CPU::RRC_r8(uint8_t *r) {
    uint8_t b7 = *r & 1;
    *r = (b7 << 7) | (*r >> 1);
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 8;
}

void CPU::RRC_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t b7 = pHL & 1;
    pHL = (b7 << 7) | (pHL >> 1);
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 16;
}

void CPU::RRCA() {
    uint8_t b7 = registers.A & 1;
    registers.A = (b7 << 7) | (registers.A >> 1);
    registers.set_Z(0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 4;
}

void CPU::SLA_r8(uint8_t *r) {
    uint8_t b7 = (*r & 0x80) >> 7;
    *r <<= 1;
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 8;
}

void CPU::SLA_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t b7 = (pHL & 0x80) >> 7;
    pHL <<= 1;
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b7);
    t_cycles += 16;
}

void CPU::SRA_r8(uint8_t *r) {
    uint8_t b7 = *r & 0x80;
    uint8_t b0 = *r & 1;
    *r = b7 | (*r >> 1);
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b0);
    t_cycles += 8;
}

void CPU::SRA_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t b7 = pHL & 0x80;
    uint8_t b0 = pHL & 1;
    pHL = b7 | (pHL >> 1);
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b0);
    t_cycles += 16;
}

void CPU::SRL_r8(uint8_t *r) {
    uint8_t b0 = *r & 1;
    *r >>= 1;
    registers.set_Z(*r == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b0);
    t_cycles += 8;
}

void CPU::SRL_pHL() {
    uint8_t pHL = mmu.read_byte(registers.get_HL());
    uint8_t b0 = pHL & 1;
    pHL >>= 1;
    mmu.write_byte(registers.get_HL(), pHL);
    registers.set_Z(pHL == 0);
    registers.set_N(0);
    registers.set_H(0);
    registers.set_C(b0);
    t_cycles += 16;
}

// Returns opcode at program_counter
uint16_t CPU::next_opcode() {
    uint8_t opcode = mmu.read_byte(program_counter++);

    if(halt_bug){
        program_counter--;
        halt_bug = false;
    }

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
                RLC_r8(&registers.B);
                break;
            case 0x01:
                RLC_r8(&registers.C);
                break;
            case 0x02:
                RLC_r8(&registers.D);
                break;
            case 0x03:
                RLC_r8(&registers.E);
                break;
            case 0x04:
                RLC_r8(&registers.H);
                break;
            case 0x05:
                RLC_r8(&registers.L);
                break;
            case 0x06:
                RLC_pHL();
                break;
            case 0x07:
                RLC_r8(&registers.A);
                break;
            case 0x08:
                RRC_r8(&registers.B);
                break;
            case 0x09:
                RRC_r8(&registers.C);
                break;
            case 0x0A:
                RRC_r8(&registers.D);
                break;
            case 0x0B:
                RRC_r8(&registers.E);
                break;
            case 0x0C:
                RRC_r8(&registers.H);
                break;
            case 0x0D:
                RRC_r8(&registers.L);
                break;
            case 0x0E:
                RRC_pHL();
                break;
            case 0x0F:
                RRC_r8(&registers.A);
                break;
            case 0x10:
                RL_r8(&registers.B);
                break;
            case 0x11:
                RL_r8(&registers.C);
                break;
            case 0x12:
                RL_r8(&registers.D);
                break;
            case 0x13:
                RL_r8(&registers.E);
                break;
            case 0x14:
                RL_r8(&registers.H);
                break;
            case 0x15:
                RL_r8(&registers.L);
                break;
            case 0x16:
                RL_pHL();
                break;
            case 0x17:
                RL_r8(&registers.A);
                break;
            case 0x18:
                RR_r8(&registers.B);
                break;
            case 0x19:
                RR_r8(&registers.C);
                break;
            case 0x1A:
                RR_r8(&registers.D);
                break;
            case 0x1B:
                RR_r8(&registers.E);
                break;
            case 0x1C:
                RR_r8(&registers.H);
                break;
            case 0x1D:
                RR_r8(&registers.L);
                break;
            case 0x1E:
                RR_pHL();
                break;
            case 0x1F:
                RR_r8(&registers.A);
                break;
            case 0x20:
                SLA_r8(&registers.B);
                break;
            case 0x21:
                SLA_r8(&registers.C);
                break;
            case 0x22:
                SLA_r8(&registers.D);
                break;
            case 0x23:
                SLA_r8(&registers.E);
                break;
            case 0x24:
                SLA_r8(&registers.H);
                break;
            case 0x25:
                SLA_r8(&registers.L);
                break;
            case 0x26:
                SLA_pHL();
                break;
            case 0x27:
                SLA_r8(&registers.A);
                break;
            case 0x28:
                SRA_r8(&registers.B);
                break;
            case 0x29:
                SRA_r8(&registers.C);
                break;
            case 0x2A:
                SRA_r8(&registers.D);
                break;
            case 0x2B:
                SRA_r8(&registers.E);
                break;
            case 0x2C:
                SRA_r8(&registers.H);
                break;
            case 0x2D:
                SRA_r8(&registers.L);
                break;
            case 0x2E:
                SRA_pHL();
                break;
            case 0x2F:
                SRA_r8(&registers.A);
                break;
            case 0x30:
                SWAP_r8(&registers.B);
                break;
            case 0x31:
                SWAP_r8(&registers.C);
                break;
            case 0x32:
                SWAP_r8(&registers.D);
                break;
            case 0x33:
                SWAP_r8(&registers.E);
                break;
            case 0x34:
                SWAP_r8(&registers.H);
                break;
            case 0x35:
                SWAP_r8(&registers.L);
                break;
            case 0x36:
                SWAP_pHL();
                break;
            case 0x37:
                SWAP_r8(&registers.A);
                break;
            case 0x38:
                SRL_r8(&registers.B);
                break;
            case 0x39:
                SRL_r8(&registers.C);
                break;
            case 0x3A:
                SRL_r8(&registers.D);
                break;
            case 0x3B:
                SRL_r8(&registers.E);
                break;
            case 0x3C:
                SRL_r8(&registers.H);
                break;
            case 0x3D:
                SRL_r8(&registers.L);
                break;
            case 0x3E:
                SRL_pHL();
                break;
            case 0x3F:
                SRL_r8(&registers.A);
                break;
            case 0x40:
                BIT_u3_r8(0, registers.B);
                break;
            case 0x41:
                BIT_u3_r8(0, registers.C);
                break;
            case 0x42:
                BIT_u3_r8(0, registers.D);
                break;
            case 0x43:
                BIT_u3_r8(0, registers.E);
                break;
            case 0x44:
                BIT_u3_r8(0, registers.H);
                break;
            case 0x45:
                BIT_u3_r8(0, registers.L);
                break;
            case 0x46:
                BIT_u3_pHL(0);
                break;
            case 0x47:
                BIT_u3_r8(0, registers.A);
                break;
            case 0x48:
                BIT_u3_r8(1, registers.B);
                break;
            case 0x49:
                BIT_u3_r8(1, registers.C);
                break;
            case 0x4A:
                BIT_u3_r8(1, registers.D);
                break;
            case 0x4B:
                BIT_u3_r8(1, registers.E);
                break;
            case 0x4C:
                BIT_u3_r8(1, registers.H);
                break;
            case 0x4D:
                BIT_u3_r8(1, registers.L);
                break;
            case 0x4E:
                BIT_u3_pHL(1);
                break;
            case 0x4F:
                BIT_u3_r8(1, registers.A);
                break;
            case 0x50:
                BIT_u3_r8(2, registers.B);
                break;
            case 0x51:
                BIT_u3_r8(2, registers.C);
                break;
            case 0x52:
                BIT_u3_r8(2, registers.D);
                break;
            case 0x53:
                BIT_u3_r8(2, registers.E);
                break;
            case 0x54:
                BIT_u3_r8(2, registers.H);
                break;
            case 0x55:
                BIT_u3_r8(2, registers.L);
                break;
            case 0x56:
                BIT_u3_pHL(2);
                break;
            case 0x57:
                BIT_u3_r8(2, registers.A);
                break;
            case 0x58:
                BIT_u3_r8(3, registers.B);
                break;
            case 0x59:
                BIT_u3_r8(3, registers.C);
                break;
            case 0x5A:
                BIT_u3_r8(3, registers.D);
                break;
            case 0x5B:
                BIT_u3_r8(3, registers.E);
                break;
            case 0x5C:
                BIT_u3_r8(3, registers.H);
                break;
            case 0x5D:
                BIT_u3_r8(3, registers.L);
                break;
            case 0x5E:
                BIT_u3_pHL(3);
                break;
            case 0x5F:
                BIT_u3_r8(3, registers.A);
                break;
            case 0x60:
                BIT_u3_r8(4, registers.B);
                break;
            case 0x61:
                BIT_u3_r8(4, registers.C);
                break;
            case 0x62:
                BIT_u3_r8(4, registers.D);
                break;
            case 0x63:
                BIT_u3_r8(4, registers.E);
                break;
            case 0x64:
                BIT_u3_r8(4, registers.H);
                break;
            case 0x65:
                BIT_u3_r8(4, registers.L);
                break;
            case 0x66:
                BIT_u3_pHL(4);
                break;
            case 0x67:
                BIT_u3_r8(4, registers.A);
                break;
            case 0x68:
                BIT_u3_r8(5, registers.B);
                break;
            case 0x69:
                BIT_u3_r8(5, registers.C);
                break;
            case 0x6A:
                BIT_u3_r8(5, registers.D);
                break;
            case 0x6B:
                BIT_u3_r8(5, registers.E);
                break;
            case 0x6C:
                BIT_u3_r8(5, registers.H);
                break;
            case 0x6D:
                BIT_u3_r8(5, registers.L);
                break;
            case 0x6E:
                BIT_u3_pHL(5);
                break;
            case 0x6F:
                BIT_u3_r8(5, registers.A);
                break;
            case 0x70:
                BIT_u3_r8(6, registers.B);
                break;
            case 0x71:
                BIT_u3_r8(6, registers.C);
                break;
            case 0x72:
                BIT_u3_r8(6, registers.D);
                break;
            case 0x73:
                BIT_u3_r8(6, registers.E);
                break;
            case 0x74:
                BIT_u3_r8(6, registers.H);
                break;
            case 0x75:
                BIT_u3_r8(6, registers.L);
                break;
            case 0x76:
                BIT_u3_pHL(6);
                break;
            case 0x77:
                BIT_u3_r8(6, registers.A);
                break;
            case 0x78:
                BIT_u3_r8(7, registers.B);
                break;
            case 0x79:
                BIT_u3_r8(7, registers.C);
                break;
            case 0x7A:
                BIT_u3_r8(7, registers.D);
                break;
            case 0x7B:
                BIT_u3_r8(7, registers.E);
                break;
            case 0x7C:
                BIT_u3_r8(7, registers.H);
                break;
            case 0x7D:
                BIT_u3_r8(7, registers.L);
                break;
            case 0x7E:
                BIT_u3_pHL(7);
                break;
            case 0x7F:
                BIT_u3_r8(7, registers.A);
                break;
            case 0x80:
                RES_u3_r8(0, &registers.B);
                break;
            case 0x81:
                RES_u3_r8(0, &registers.C);
                break;
            case 0x82:
                RES_u3_r8(0, &registers.D);
                break;
            case 0x83:
                RES_u3_r8(0, &registers.E);
                break;
            case 0x84:
                RES_u3_r8(0, &registers.H);
                break;
            case 0x85:
                RES_u3_r8(0, &registers.L);
                break;
            case 0x86:
                RES_u3_pHL(0);
                break;
            case 0x87:
                RES_u3_r8(0, &registers.A);
                break;
            case 0x88:
                RES_u3_r8(1, &registers.B);
                break;
            case 0x89:
                RES_u3_r8(1, &registers.C);
                break;
            case 0x8A:
                RES_u3_r8(1, &registers.D);
                break;
            case 0x8B:
                RES_u3_r8(1, &registers.E);
                break;
            case 0x8C:
                RES_u3_r8(1, &registers.H);
                break;
            case 0x8D:
                RES_u3_r8(1, &registers.L);
                break;
            case 0x8E:
                RES_u3_pHL(1);
                break;
            case 0x8F:
                RES_u3_r8(1, &registers.A);
                break;
            case 0x90:
                RES_u3_r8(2, &registers.B);
                break;
            case 0x91:
                RES_u3_r8(2, &registers.C);
                break;
            case 0x92:
                RES_u3_r8(2, &registers.D);
                break;
            case 0x93:
                RES_u3_r8(2, &registers.E);
                break;
            case 0x94:
                RES_u3_r8(2, &registers.H);
                break;
            case 0x95:
                RES_u3_r8(2, &registers.L);
                break;
            case 0x96:
                RES_u3_pHL(2);
                break;
            case 0x97:
                RES_u3_r8(2, &registers.A);
                break;
            case 0x98:
                RES_u3_r8(3, &registers.B);
                break;
            case 0x99:
                RES_u3_r8(3, &registers.C);
                break;
            case 0x9A:
                RES_u3_r8(3, &registers.D);
                break;
            case 0x9B:
                RES_u3_r8(3, &registers.E);
                break;
            case 0x9C:
                RES_u3_r8(3, &registers.H);
                break;
            case 0x9D:
                RES_u3_r8(3, &registers.L);
                break;
            case 0x9E:
                RES_u3_pHL(3);
                break;
            case 0x9F:
                RES_u3_r8(3, &registers.A);
                break;
            case 0xA0:
                RES_u3_r8(4, &registers.B);
                break;
            case 0xA1:
                RES_u3_r8(4, &registers.C);
                break;
            case 0xA2:
                RES_u3_r8(4, &registers.D);
                break;
            case 0xA3:
                RES_u3_r8(4, &registers.E);
                break;
            case 0xA4:
                RES_u3_r8(4, &registers.H);
                break;
            case 0xA5:
                RES_u3_r8(4, &registers.L);
                break;
            case 0xA6:
                RES_u3_pHL(4);
                break;
            case 0xA7:
                RES_u3_r8(4, &registers.A);
                break;
            case 0xA8:
                RES_u3_r8(5, &registers.B);
                break;
            case 0xA9:
                RES_u3_r8(5, &registers.C);
                break;
            case 0xAA:
                RES_u3_r8(5, &registers.D);
                break;
            case 0xAB:
                RES_u3_r8(5, &registers.E);
                break;
            case 0xAC:
                RES_u3_r8(5, &registers.H);
                break;
            case 0xAD:
                RES_u3_r8(5, &registers.L);
                break;
            case 0xAE:
                RES_u3_pHL(5);
                break;
            case 0xAF:
                RES_u3_r8(5, &registers.A);
                break;
            case 0xB0:
                RES_u3_r8(6, &registers.B);
                break;
            case 0xB1:
                RES_u3_r8(6, &registers.C);
                break;
            case 0xB2:
                RES_u3_r8(6, &registers.D);
                break;
            case 0xB3:
                RES_u3_r8(6, &registers.E);
                break;
            case 0xB4:
                RES_u3_r8(6, &registers.H);
                break;
            case 0xB5:
                RES_u3_r8(6, &registers.L);
                break;
            case 0xB6:
                RES_u3_pHL(6);
                break;
            case 0xB7:
                RES_u3_r8(6, &registers.A);
                break;
            case 0xB8:
                RES_u3_r8(7, &registers.B);
                break;
            case 0xB9:
                RES_u3_r8(7, &registers.C);
                break;
            case 0xBA:
                RES_u3_r8(7, &registers.D);
                break;
            case 0xBB:
                RES_u3_r8(7, &registers.E);
                break;
            case 0xBC:
                RES_u3_r8(7, &registers.H);
                break;
            case 0xBD:
                RES_u3_r8(7, &registers.L);
                break;
            case 0xBE:
                RES_u3_pHL(7);
                break;
            case 0xBF:
                RES_u3_r8(7, &registers.A);
                break;
            case 0xC0:
                SET_u3_r8(0, &registers.B);
                break;
            case 0xC1:
                SET_u3_r8(0, &registers.C);
                break;
            case 0xC2:
                SET_u3_r8(0, &registers.D);
                break;
            case 0xC3:
                SET_u3_r8(0, &registers.E);
                break;
            case 0xC4:
                SET_u3_r8(0, &registers.H);
                break;
            case 0xC5:
                SET_u3_r8(0, &registers.L);
                break;
            case 0xC6:
                SET_u3_pHL(0);
                break;
            case 0xC7:
                SET_u3_r8(0, &registers.A);
                break;
            case 0xC8:
                SET_u3_r8(1, &registers.B);
                break;
            case 0xC9:
                SET_u3_r8(1, &registers.C);
                break;
            case 0xCA:
                SET_u3_r8(1, &registers.D);
                break;
            case 0xCB:
                SET_u3_r8(1, &registers.E);
                break;
            case 0xCC:
                SET_u3_r8(1, &registers.H);
                break;
            case 0xCD:
                SET_u3_r8(1, &registers.L);
                break;
            case 0xCE:
                SET_u3_pHL(1);
                break;
            case 0xCF:
                SET_u3_r8(1, &registers.A);
                break;
            case 0xD0:
                SET_u3_r8(2, &registers.B);
                break;
            case 0xD1:
                SET_u3_r8(2, &registers.C);
                break;
            case 0xD2:
                SET_u3_r8(2, &registers.D);
                break;
            case 0xD3:
                SET_u3_r8(2, &registers.E);
                break;
            case 0xD4:
                SET_u3_r8(2, &registers.H);
                break;
            case 0xD5:
                SET_u3_r8(2, &registers.L);
                break;
            case 0xD6:
                SET_u3_pHL(2);
                break;
            case 0xD7:
                SET_u3_r8(2, &registers.A);
                break;
            case 0xD8:
                SET_u3_r8(3, &registers.B);
                break;
            case 0xD9:
                SET_u3_r8(3, &registers.C);
                break;
            case 0xDA:
                SET_u3_r8(3, &registers.D);
                break;
            case 0xDB:
                SET_u3_r8(3, &registers.E);
                break;
            case 0xDC:
                SET_u3_r8(3, &registers.H);
                break;
            case 0xDD:
                SET_u3_r8(3, &registers.L);
                break;
            case 0xDE:
                SET_u3_pHL(3);
                break;
            case 0xDF:
                SET_u3_r8(3, &registers.A);
                break;
            case 0xE0:
                SET_u3_r8(4, &registers.B);
                break;
            case 0xE1:
                SET_u3_r8(4, &registers.C);
                break;
            case 0xE2:
                SET_u3_r8(4, &registers.D);
                break;
            case 0xE3:
                SET_u3_r8(4, &registers.E);
                break;
            case 0xE4:
                SET_u3_r8(4, &registers.H);
                break;
            case 0xE5:
                SET_u3_r8(4, &registers.L);
                break;
            case 0xE6:
                SET_u3_pHL(4);
                break;
            case 0xE7:
                SET_u3_r8(4, &registers.A);
                break;
            case 0xE8:
                SET_u3_r8(5, &registers.B);
                break;
            case 0xE9:
                SET_u3_r8(5, &registers.C);
                break;
            case 0xEA:
                SET_u3_r8(5, &registers.D);
                break;
            case 0xEB:
                SET_u3_r8(5, &registers.E);
                break;
            case 0xEC:
                SET_u3_r8(5, &registers.H);
                break;
            case 0xED:
                SET_u3_r8(5, &registers.L);
                break;
            case 0xEE:
                SET_u3_pHL(5);
                break;
            case 0xEF:
                SET_u3_r8(5, &registers.A);
                break;
            case 0xF0:
                SET_u3_r8(6, &registers.B);
                break;
            case 0xF1:
                SET_u3_r8(6, &registers.C);
                break;
            case 0xF2:
                SET_u3_r8(6, &registers.D);
                break;
            case 0xF3:
                SET_u3_r8(6, &registers.E);
                break;
            case 0xF4:
                SET_u3_r8(6, &registers.H);
                break;
            case 0xF5:
                SET_u3_r8(6, &registers.L);
                break;
            case 0xF6:
                SET_u3_pHL(6);
                break;
            case 0xF7:
                SET_u3_r8(6, &registers.A);
                break;
            case 0xF8:
                SET_u3_r8(7, &registers.B);
                break;
            case 0xF9:
                SET_u3_r8(7, &registers.C);
                break;
            case 0xFA:
                SET_u3_r8(7, &registers.D);
                break;
            case 0xFB:
                SET_u3_r8(7, &registers.E);
                break;
            case 0xFC:
                SET_u3_r8(7, &registers.H);
                break;
            case 0xFD:
                SET_u3_r8(7, &registers.L);
                break;
            case 0xFE:
                SET_u3_pHL(7);
                break;
            case 0xFF:
                SET_u3_r8(7, &registers.A);
                break;
        }
    }else{
        // Non CB-prefixed opcodes
        switch (opcode & 0xFF) {
            case 0x00:
                NOP();
                break;
            case 0x01:
                LD_r16_u16(&registers.B, &registers.C);
                break;
            case 0x02:
                LD_pr16_A(registers.get_BC());
                break;
            case 0x03:
                INC_r16(&registers.B, &registers.C);
                break;
            case 0x04:
                INC_r8(&registers.B);
                break;
            case 0x05:
                DEC_r8(&registers.B);
                break;
            case 0x06:
                LD_r8_u8(&registers.B);
                break;
            case 0x07:
                RLCA();
                break;
            case 0x08:
                LD_pu16_SP();
                break;
            case 0x09:
                ADD_HL_r16(registers.get_BC());
                break;
            case 0x0A:
                LD_A_pr16(registers.get_BC());
                break;
            case 0x0B:
                DEC_r16(&registers.B, &registers.C);
                break;
            case 0x0C:
                INC_r8(&registers.C);
                break;
            case 0x0D:
                DEC_r8(&registers.C);
                break;
            case 0x0E:
                LD_r8_u8(&registers.C);
                break;
            case 0x0F:
                RRCA();
                break;
            case 0x10:
                STOP();
                break;
            case 0x11:
                LD_r16_u16(&registers.D, &registers.E);
                break;
            case 0x12:
                LD_pr16_A(registers.get_DE());
                break;
            case 0x13:
                INC_r16(&registers.D, &registers.E);
                break;
            case 0x14:
                INC_r8(&registers.D);
                break;
            case 0x15:
                DEC_r8(&registers.D);
                break;
            case 0x16:
                LD_r8_u8(&registers.D);
                break;
            case 0x17:
                RLA();
                break;
            case 0x18:
                JR_i8();
                break;
            case 0x19:
                ADD_HL_r16(registers.get_DE());
                break;
            case 0x1A:
                LD_A_pr16(registers.get_DE());
                break;
            case 0x1B:
                DEC_r16(&registers.D, &registers.E);
                break;
            case 0x1C:
                INC_r8(&registers.E);
                break;
            case 0x1D:
                DEC_r8(&registers.E);
                break;
            case 0x1E:
                LD_r8_u8(&registers.E);
                break;
            case 0x1F:
                RRA();
                break;
            case 0x20:
                JR_cc_i8(!registers.get_Z());
                break;
            case 0x21:
                LD_r16_u16(&registers.H, &registers.L);
                break;
            case 0x22:
                LD_pHLI_A();
                break;
            case 0x23:
                INC_r16(&registers.H, &registers.L);
                break;
            case 0x24:
                INC_r8(&registers.H);
                break;
            case 0x25:
                DEC_r8(&registers.H);
                break;
            case 0x26:
                LD_r8_u8(&registers.H);
                break;
            case 0x27:
                DAA();
                break;
            case 0x28:
                JR_cc_i8(registers.get_Z());
                break;
            case 0x29:
                ADD_HL_r16(registers.get_HL());
                break;
            case 0x2A:
                LD_A_pHLI();
                break;
            case 0x2B:
                DEC_r16(&registers.H, &registers.L);
                break;
            case 0x2C:
                INC_r8(&registers.L);
                break;
            case 0x2D:
                DEC_r8(&registers.L);
                break;
            case 0x2E:
                LD_r8_u8(&registers.L);
                break;
            case 0x2F:
                CPL();
                break;
            case 0x30:
                JR_cc_i8(!registers.get_C());
                break;
            case 0x31:
                LD_SP_u16();
                break;
            case 0x32:
                LD_pHLD_A();
                break;
            case 0x33:
                INC_SP();
                break;
            case 0x34:
                INC_pHL();
                break;
            case 0x35:
                DEC_pHL();
                break;
            case 0x36:
                LD_pHL_n8();
                break;
            case 0x37:
                SCF();
                break;
            case 0x38:
                JR_cc_i8(registers.get_C());
                break;
            case 0x39:
                ADD_HL_r16(stack_pointer);
                break;
            case 0x3A:
                LD_A_pHLD();
                break;
            case 0x3B:
                DEC_SP();
                break;
            case 0x3C:
                INC_r8(&registers.A);
                break;
            case 0x3D:
                DEC_r8(&registers.A);
                break;
            case 0x3E:
                LD_r8_u8(&registers.A);
                break;
            case 0x3F:
                CCF();
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
                HALT();
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
                ADD_A_r8(registers.B);
                break;
            case 0x81:
                ADD_A_r8(registers.C);
                break;
            case 0x82:
                ADD_A_r8(registers.D);
                break;
            case 0x83:
                ADD_A_r8(registers.E);
                break;
            case 0x84:
                ADD_A_r8(registers.H);
                break;
            case 0x85:
                ADD_A_r8(registers.L);
                break;
            case 0x86:
                ADD_A_pHL();
                break;
            case 0x87:
                ADD_A_r8(registers.A);
                break;
            case 0x88:
                ADC_A_r8(registers.B);
                break;
            case 0x89:
                ADC_A_r8(registers.C);
                break;
            case 0x8A:
                ADC_A_r8(registers.D);
                break;
            case 0x8B:
                ADC_A_r8(registers.E);
                break;
            case 0x8C:
                ADC_A_r8(registers.H);
                break;
            case 0x8D:
                ADC_A_r8(registers.L);
                break;
            case 0x8E:
                ADC_A_pHL();
                break;
            case 0x8F:
                ADC_A_r8(registers.A);
                break;
            case 0x90:
                SUB_A_r8(registers.B);
                break;
            case 0x91:
                SUB_A_r8(registers.C);
                break;
            case 0x92:
                SUB_A_r8(registers.D);
                break;
            case 0x93:
                SUB_A_r8(registers.E);
                break;
            case 0x94:
                SUB_A_r8(registers.H);
                break;
            case 0x95:
                SUB_A_r8(registers.L);
                break;
            case 0x96:
                SUB_A_pHL();
                break;
            case 0x97:
                SUB_A_r8(registers.A);
                break;
            case 0x98:
                SBC_A_r8(registers.B);
                break;
            case 0x99:
                SBC_A_r8(registers.C);
                break;
            case 0x9A:
                SBC_A_r8(registers.D);
                break;
            case 0x9B:
                SBC_A_r8(registers.E);
                break;
            case 0x9C:
                SBC_A_r8(registers.H);
                break;
            case 0x9D:
                SBC_A_r8(registers.L);
                break;
            case 0x9E:
                SBC_A_pHL();
                break;
            case 0x9F:
                SBC_A_r8(registers.A);
                break;
            case 0xA0:
                AND_A_r8(registers.B);
                break;
            case 0xA1:
                AND_A_r8(registers.C);
                break;
            case 0xA2:
                AND_A_r8(registers.D);
                break;
            case 0xA3:
                AND_A_r8(registers.E);
                break;
            case 0xA4:
                AND_A_r8(registers.H);
                break;
            case 0xA5:
                AND_A_r8(registers.L);
                break;
            case 0xA6:
                AND_A_pHL();
                break;
            case 0xA7:
                AND_A_r8(registers.A);
                break;
            case 0xA8:
                XOR_A_r8(registers.B);
                break;
            case 0xA9:
                XOR_A_r8(registers.C);
                break;
            case 0xAA:
                XOR_A_r8(registers.D);
                break;
            case 0xAB:
                XOR_A_r8(registers.E);
                break;
            case 0xAC:
                XOR_A_r8(registers.H);
                break;
            case 0xAD:
                XOR_A_r8(registers.L);
                break;
            case 0xAE:
                XOR_A_pHL();
                break;
            case 0xAF:
                XOR_A_r8(registers.A);
                break;
            case 0xB0:
                OR_A_r8(registers.B);
                break;
            case 0xB1:
                OR_A_r8(registers.C);
                break;
            case 0xB2:
                OR_A_r8(registers.D);
                break;
            case 0xB3:
                OR_A_r8(registers.E);
                break;
            case 0xB4:
                OR_A_r8(registers.H);
                break;
            case 0xB5:
                OR_A_r8(registers.L);
                break;
            case 0xB6:
                OR_A_pHL();
                break;
            case 0xB7:
                OR_A_r8(registers.A);
                break;
            case 0xB8:
                CP_A_r8(registers.B);
                break;
            case 0xB9:
                CP_A_r8(registers.C);
                break;
            case 0xBA:
                CP_A_r8(registers.D);
                break;
            case 0xBB:
                CP_A_r8(registers.E);
                break;
            case 0xBC:
                CP_A_r8(registers.H);
                break;
            case 0xBD:
                CP_A_r8(registers.L);
                break;
            case 0xBE:
                CP_A_pHL();
                break;
            case 0xBF:
                CP_A_r8(registers.A);
                break;
            case 0xC0:
                RET_cc(!registers.get_Z());
                break;
            case 0xC1:
                POP_r16(&registers.B, &registers.C);
                break;
            case 0xC2:
                JP_cc_u16(!registers.get_Z());
                break;
            case 0xC3:
                JP_u16();
                break;
            case 0xC4:
                CALL_cc_u16(!registers.get_Z());
                break;
            case 0xC5:
                PUSH_r16(registers.get_BC());
                break;
            case 0xC6:
                ADD_A_u8();
                break;
            case 0xC7:
                RST_vec(0x00);
                break;
            case 0xC8:
                RET_cc(registers.get_Z());
                break;
            case 0xC9:
                RET();
                break;
            case 0xCA:
                JP_cc_u16(registers.get_Z());
                break;
            case 0xCB:
                // UNUSED (CB prefix handled in next_opcode())
                break;
            case 0xCC:
                CALL_cc_u16(registers.get_Z());
                break;
            case 0xCD:
                CALL_u16();
                break;
            case 0xCE:
                ADC_A_u8();
                break;
            case 0xCF:
                RST_vec(0x08);
                break;
            case 0xD0:
                RET_cc(!registers.get_C());
                break;
            case 0xD1:
                POP_r16(&registers.D, &registers.E);
                break;
            case 0xD2:
                JP_cc_u16(!registers.get_C());
                break;
            case 0xD3:
                // UNUSED
                break;
            case 0xD4:
                CALL_cc_u16(!registers.get_C());
                break;
            case 0xD5:
                PUSH_r16(registers.get_DE());
                break;
            case 0xD6:
                SUB_A_u8();
                break;
            case 0xD7:
                RST_vec(0x10);
                break;
            case 0xD8:
                RET_cc(registers.get_C());
                break;
            case 0xD9:
                RETI();
                break;
            case 0xDA:
                JP_cc_u16(registers.get_C());
                break;
            case 0xDB:
                // UNUSED
                break;
            case 0xDC:
                CALL_cc_u16(registers.get_C());
                break;
            case 0xDD:
                // UNUSED
                break;
            case 0xDE:
                SBC_A_u8();
                break;
            case 0xDF:
                RST_vec(0x18);
                break;
            case 0xE0:
                LD_pFF00u8_A();
                break;
            case 0xE1:
                POP_r16(&registers.H, &registers.L);
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
                PUSH_r16(registers.get_HL());
                break;
            case 0xE6:
                AND_A_u8();
                break;
            case 0xE7:
                RST_vec(0x20);
                break;
            case 0xE8:
                ADD_SP_i8();
                break;
            case 0xE9:
                JP_HL();
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
                XOR_A_u8();
                break;
            case 0xEF:
                RST_vec(0x28);
                break;
            case 0xF0:
                LD_A_pFF00u8();
                break;
            case 0xF1:
                POP_AF();
                break;
            case 0xF2:
                LD_A_pC();
                break;
            case 0xF3:
                DI();
                break;
            case 0xF4:
                // UNUSED
                break;
            case 0xF5:
                PUSH_AF();
                break;
            case 0xF6:
                OR_A_u8();
                break;
            case 0xF7:
                RST_vec(0x30);
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
                EI();
                break;
            case 0xFC:
                // UNUSED
                break;
            case 0xFD:
                // UNUSED
                break;
            case 0xFE:
                CP_A_u8();
                break;
            case 0xFF:
                RST_vec(0x38);
                break;
        }
    }
}

void CPU::detect_interrupt() {
    uint8_t IE = mmu.read_byte(0xFFFF);
    uint8_t IF = mmu.read_byte(0xFF0F);
    if(IME){
        // VBlank
        if((IE & IF & 0b1) == 0b1){
            process_interrupt(&IF, 0);
        }
        // LCD
        else if((IE & IF & 0b10) == 0b10){
            process_interrupt(&IF, 1);
        }
        // Timer
        else if((IE & IF & 0b100) == 0b100){
            process_interrupt(&IF, 2);
        }
        // Serial
        else if((IE & IF & 0b1000) == 0b1000){
            process_interrupt(&IF, 3);
        }
        // Joypad
        else if((IE & IF & 0b10000) == 0b10000){
            process_interrupt(&IF, 4);
        }
    }
}

// Executed when an interrupt is pending
void CPU::process_interrupt(uint8_t* IF, int i) {
    // Wake if halted/stopped
    halted = false;
    stopped = false;
    if(skip_handler){
        skip_handler = false;
        return;
    }

    // Clear interrupt flags
    *IF &= ~(1 << i);
    mmu.write_byte(0xFF0F, *IF);
    IME = false;

    // Call interrupt handler
    mmu.write_byte(--stack_pointer, program_counter & 0xFF00);
    mmu.write_byte(--stack_pointer, program_counter & 0xFF);
    program_counter = 0x40 + i * 8;
    t_cycles += 20;
}