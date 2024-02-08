#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>

#ifndef GAMEBOY_COLOR_EMULATOR_MMU_H
#define GAMEBOY_COLOR_EMULATOR_MMU_H


class MMU {
public:
    uint8_t gb_memory[0x10000];
    uint8_t cartridge_memory[0x200000];

    MMU();
    void load_ROM(std::string exec_path, std::string file_name);

    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t val);
};


#endif //GAMEBOY_COLOR_EMULATOR_MMU_H
