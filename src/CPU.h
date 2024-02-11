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

    // Load instructions
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

    // Arithmetic and logic instructions
    void ADC_A_r8(uint8_t r);
    void ADC_A_pHL();
    void ADC_A_u8();
    void ADD_A_r8(uint8_t r);
    void ADD_A_pHL();
    void ADD_A_u8();
    void ADD_HL_r16(uint16_t r);
    void ADD_SP_i8();
    void AND_A_r8(uint8_t r);
    void AND_A_pHL();
    void AND_A_u8();
    void CP_A_r8(uint8_t r);
    void CP_A_pHL();
    void CP_A_u8();
    void DEC_r8(uint8_t* r);
    void DEC_pHL();
    void DEC_SP();
    void DEC_r16(uint8_t* r1, uint8_t* r2);
    void INC_r8(uint8_t* r);
    void INC_pHL();
    void INC_SP();
    void INC_r16(uint8_t* r1, uint8_t* r2);
    void OR_A_r8(uint8_t r);
    void OR_A_pHL();
    void OR_A_u8();
    void SBC_A_r8(uint8_t r);
    void SBC_A_pHL();
    void SBC_A_u8();
    void SUB_A_r8(uint8_t r);
    void SUB_A_pHL();
    void SUB_A_u8();
    void XOR_A_r8(uint8_t r);
    void XOR_A_pHL();
    void XOR_A_u8();

    // Jumps and subroutines
    void CALL_u16();
    void CALL_cc_u16();
    void JP_HL();
    void JP_u16();
    void JP_cc_u16();
    void JR_u16();
    void JR_cc_u16();
    void RET_cc();
    void RET();
    void RETI();
    void RST_vec();

    // Stack operations
    void POP_AF();
    void POP_r16();
    void PUSH_AF();
    void PUSH_r16();

    // Miscellaneous instructions
    void CCF();
    void CPL();
    void DAA();
    void DI();
    void EI();
    void HALT();
    void NOP();
    void SCF();
    void STOP();

    // Bit operations
    void BIT_u3_r8();
    void BIT_u3_pHL();
    void RES_u3_r8();
    void RES_u3_pHL();
    void SET_u3_r8();
    void SET_u3_pHL();
    void SWAP_r8();
    void SWAP_pHL();

    // Bit shifts
    void RL_r8();
    void RL_pHL();
    void RLA();
    void RLC_r8();
    void RLC_pHL();
    void RLCA();
    void RR_r8();
    void RR_pHL();
    void RRA();
    void RRC_r8();
    void RRC_pHL();
    void RRCA();
    void SLA_r8();
    void SLA_pHL();
    void SRA_r8();
    void SRA_pHL();
    void SRL_r8();
    void SRL_pHL();
};


#endif //GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
