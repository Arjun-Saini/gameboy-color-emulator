#ifndef GAMEBOY_COLOR_EMULATOR_REGISTERS_H
#define GAMEBOY_COLOR_EMULATOR_REGISTERS_H

#include <cstdint>

class Registers {
public:
    Registers();

    uint8_t A;
    uint8_t F;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;

    uint16_t get_AF();
    uint16_t get_BC();
    uint16_t get_DE();
    uint16_t get_HL();

    void set_AF(uint16_t val);
    void set_BC(uint16_t val);
    void set_DE(uint16_t val);
    void set_HL(uint16_t val);

    uint8_t get_Z();
    uint8_t get_N();
    uint8_t get_H();
    uint8_t get_C();

    void set_Z(uint8_t val);
    void set_N(uint8_t val);
    void set_H(uint8_t val);
    void set_C(uint8_t val);

    void b8_carry(int v1, int v2);
    void b8_carry(int v1, int v2, int v3);
    void b16_carry(int v1, int v2);
    void b4_half_carry(int v1, int v2);
    void b4_half_carry(int v1, int v2, int v3);
    void b12_half_carry(int v1, int v2);
};


#endif //GAMEBOY_COLOR_EMULATOR_REGISTERS_H
