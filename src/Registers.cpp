#include "Registers.h"

Registers::Registers(){
    A = 0;
    F = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    H = 0;
    L = 0;
}

uint16_t Registers::get_AF() {
    return (A << 8) | F;
}

uint16_t Registers::get_BC() {
    return (B << 8) | C;
}

uint16_t Registers::get_DE() {
    return (D << 8) | E;
}

uint16_t Registers::get_HL() {
    return (H << 8) | L;
}

void Registers::set_AF(uint16_t val) {
    A = (val & 0xFF00) >> 8;
    F = val & 0xFF;
}

void Registers::set_BC(uint16_t val) {
    B = (val & 0xFF00) >> 8;
    C = val & 0xFF;
}

void Registers::set_DE(uint16_t val) {
    D = (val & 0xFF00) >> 8;
    E = val & 0xFF;
}

void Registers::set_HL(uint16_t val) {
    H = (val & 0xFF00) >> 8;
    L = val & 0xFF;
}

uint8_t Registers::get_Z() {
    return F >> 7;
}

uint8_t Registers::get_N() {
    return (F >> 6) & 0b1;
}

uint8_t Registers::get_H() {
    return (F >> 5) & 0b1;
}

uint8_t Registers::get_C() {
    return (F >> 4) & 0b1;
}

void Registers::set_Z(uint8_t val) {
    F &= ~(1 << 7);
    F |= (val << 7);
}

void Registers::set_N(uint8_t val) {
    F &= ~(1 << 6);
    F |= (val << 6);
}

void Registers::set_H(uint8_t val) {
    F &= ~(1 << 5);
    F |= (val << 5);
}

void Registers::set_C(uint8_t val) {
    F &= ~(1 << 4);
    F |= (val << 4);
}

void Registers::b8_carry(int v1, int v2) {
    if(((v1 & 0xFF) + (v2 & 0xFF)) & 0x100){
        set_C(1);
    }else{
        set_C(0);
    }
}

void Registers::b4_half_carry(int v1, int v2) {
    if(((v1 & 0xF) + (v2 & 0xF)) & 0x10){
        set_H(1);
    }else{
        set_H(0);
    }
}