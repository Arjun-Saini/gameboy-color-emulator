#include <iostream>
#include "MMU.h"
#include "Registers.h"

#ifndef GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
#define GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H

class CPU {
public:
    typedef int (CPU::*opcode_func)(uint16_t, uint16_t);

    uint16_t program_counter;
    uint16_t stack_pointer;

    Registers registers;
    MMU mmu;

    uint8_t t_cycles;
    uint32_t cycles_per_frame;

    CPU();

    uint16_t next_opcode();
    void decode_opcode(uint16_t opcode);

    void LD_r8_r8(uint8_t* r1, uint8_t r2);
    void LD_r8_u8(uint8_t* r);
    void LD_r16_u16(uint8_t* r1, uint8_t* r2);
    void LD_pHL_r8(uint8_t r);
    void LD_pHL_n8();
    void LD_r8_pHL(uint8_t* r);
    void LD_pr16_A(uint16_t r);
    void LD_pu16_A();
    void LD_pFF00u8_A();
    void LD_pC_A();
    void LD_A_pr16(uint16_t r);
    void LD_A_pu16();
    void LD_A_pFF00u8();
    void LD_A_pC();
    void LD_pHLI_A();
    void LD_pHLD_A();
    void LD_A_pHLD();
    void LD_A_pHLI();
    void LD_SP_u16();
    void LD_pu16_SP();
    void LD_HL_SPi8();
    void LD_SP_HL();


};


#endif //GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
