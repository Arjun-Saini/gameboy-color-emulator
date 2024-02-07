#include <cstdint>

#ifndef GAMEBOY_COLOR_EMULATOR_REGISTERS_H
#define GAMEBOY_COLOR_EMULATOR_REGISTERS_H

class Registers {
public:
    Registers();

    uint8_t reg_A;
    uint8_t reg_F;
    uint8_t reg_B;
    uint8_t reg_C;
    uint8_t reg_D;
    uint8_t reg_E;
    uint8_t reg_H;
    uint8_t reg_L;

    uint16_t get_reg_AF();
    uint16_t get_reg_BC();
    uint16_t get_reg_DE();
    uint16_t get_reg_HL();

    void set_reg_AF(uint16_t val);
    void set_reg_BC(uint16_t val);
    void set_reg_DE(uint16_t val);
    void set_reg_HL(uint16_t val);

    uint8_t get_flag_Z();
    uint8_t get_flag_N();
    uint8_t get_flag_H();
    uint8_t get_flag_C();

    void set_flag_Z(uint8_t val);
    void set_flag_N(uint8_t val);
    void set_flag_H(uint8_t val);
    void set_flag_C(uint8_t val);
};


#endif //GAMEBOY_COLOR_EMULATOR_REGISTERS_H
