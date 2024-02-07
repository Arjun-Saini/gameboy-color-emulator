#include "Registers.h"

Registers::Registers(){
    reg_A = 0;
    reg_F = 0;
    reg_B = 0;
    reg_C = 0;
    reg_D = 0;
    reg_E = 0;
    reg_H = 0;
    reg_L = 0;
}

uint16_t Registers::get_reg_AF() {
    return (reg_A << 8) | reg_F;
}

uint16_t Registers::get_reg_BC() {
    return (reg_B << 8) | reg_C;
}

uint16_t Registers::get_reg_DE() {
    return (reg_D << 8) | reg_E;
}

uint16_t Registers::get_reg_HL() {
    return (reg_H << 8) | reg_L;
}

void Registers::set_reg_AF(uint16_t val) {
    reg_A = (val & 0xFF00) >> 8;
    reg_F = val & 0xFF;
}

void Registers::set_reg_BC(uint16_t val) {
    reg_B = (val & 0xFF00) >> 8;
    reg_C = val & 0xFF;
}

void Registers::set_reg_DE(uint16_t val) {
    reg_D = (val & 0xFF00) >> 8;
    reg_E = val & 0xFF;
}

void Registers::set_reg_HL(uint16_t val) {
    reg_H = (val & 0xFF00) >> 8;
    reg_L = val & 0xFF;
}

uint8_t Registers::get_flag_Z() {
    return reg_F >> 7;
}

uint8_t Registers::get_flag_N() {
    return (reg_F >> 6) & 0b1;
}

uint8_t Registers::get_flag_H() {
    return (reg_F >> 5) & 0b1;
}

uint8_t Registers::get_flag_C() {
    return (reg_F >> 4) & 0b1;
}

void Registers::set_flag_Z(uint8_t val) {
    reg_F |= (val << 7);
}

void Registers::set_flag_N(uint8_t val) {
    reg_F |= (val << 6);
}

void Registers::set_flag_H(uint8_t val) {
    reg_F |= (val << 5);
}

void Registers::set_flag_C(uint8_t val) {
    reg_F |= (val << 4);
}