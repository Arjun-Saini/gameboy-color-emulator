#ifndef GAMEBOY_COLOR_EMULATOR_MMU_H
#define GAMEBOY_COLOR_EMULATOR_MMU_H

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Timer.h"

class MMU {
public:
    // 64 KiB internal memory
    uint8_t gb_memory[0x10000];

    // 8 MiB cartridge memory
    uint8_t* cartridge_memory = new uint8_t[0x800000]();

    // 128 KiB external RAM
    uint8_t external_RAM[0x20000];
    bool RAM_enabled = false;

    Timer* timer;

    uint8_t MBC = 0;
    uint8_t MBC_mode = 0;
    uint16_t ROM_bank = 1;
    uint8_t RAM_bank = 0;

    uint8_t RTC_index = 0;
    uint8_t RTC_reg[5];

    MMU();
    void load_ROM(std::string exec_path, std::string file_name);

    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t val);
    void update_RTC();
};


#endif //GAMEBOY_COLOR_EMULATOR_MMU_H
