#include <cstddef>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "Registers.h"

#ifndef GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
#define GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H

class GameboyCPU {
public:
    uint8_t gb_memory[0x10000];
    uint8_t cartridge_memory[0x200000];

    uint16_t program_counter;
    uint16_t stack_pointer;

    Registers registers;

    uint8_t screen[160][144][3];

    GameboyCPU();

    void load_ROM(std::string exec_path, std::string file_name);
};


#endif //GAMEBOY_COLOR_EMULATOR_GAMEBOYCPU_H
