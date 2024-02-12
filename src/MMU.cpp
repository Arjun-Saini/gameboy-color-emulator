#include "MMU.h"

MMU::MMU() {
    // Clear memory
    std::fill(std::begin(gb_memory), std::end(gb_memory), 0);
}

// Loads ROM into memory
void MMU::load_ROM(std::string exec_path, std::string file_name) {
    std::filesystem::path fs_exec_path(exec_path);
    std::string gb_path = fs_exec_path.parent_path().parent_path().string() + "/ROMs/" + file_name + ".gb";
    std::string gbc_path = fs_exec_path.parent_path().parent_path().string() + "/ROMs/" + file_name + ".gbc";

    // Open ROM
    std::ifstream ROM;
    ROM = std::ifstream(gb_path, std::ios::binary);
    if(!ROM.is_open()) {
        ROM = std::ifstream(gbc_path, std::ios::binary);
        if(!ROM.is_open()){
            std::cout << "Failed to open ROM" << std::endl;
            return;
        }
    }
    std::cout << "ROM opened successfully" << std::endl;

    ROM.seekg(0, std::ios::end);
    std::streamoff fileSize = ROM.tellg();
    ROM.seekg(0, std::ios::beg);

    // Load ROM into buffer
    char buffer[fileSize];
    ROM.read(buffer, fileSize);

    // Copy buffer into cartridge memory, load ROM bank 0 into gb memory
    for(int i = 0; i < fileSize; i++){
        cartridge_memory[i] = buffer[i];
        if(i < 0x4000){
            gb_memory[i] = cartridge_memory[i];
        }
    }

    // Select MBC cartridge type
    switch(cartridge_memory[0x147]){
        case 0x1: case 0x2: case 0x3:
            MBC = 1;
            break;
        case 0x5: case 0x6:
            MBC = 2;
            break;
        case 0xF: case 0x10: case 0x11: case 0x12: case 0x13:
            MBC = 3;
            break;
        case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E:
            MBC = 5;
            break;
    }
}

uint8_t MMU::read_byte(uint16_t addr) {
    // ROM banks 1-N
    if(0x4000 <= addr && addr <= 0x7FFF){
        return cartridge_memory[0x4000 * ROM_bank + (addr - 0x4000)];
    }

    // RAM banks 0-N
    else if(0xA000 <= addr && addr <= 0xBFFF){
        return RAM_enabled ? external_RAM[0x2000 * RAM_bank + (addr - 0xA000)] : 0;
    }

    // TODO CGB: 4 KiB WRAM banks 1-7
//    else if(0xD000 <= addr && addr <= 0xDFFF){
//
//    }

    // Prohibited
    else if(0xFEA0 <= addr && addr <= 0xFEFF){
        return 0xFF;
    }

    else{
        return gb_memory[addr];
    }

}

void MMU::write_byte(uint16_t addr, uint8_t val) {
    // RAM enable/disable
    if(0x0000 <= addr && addr <= 0x1FFF){
        RAM_enabled = (val == 0xA);
    }

    // ROM bank select
    else if(0x2000 <= addr && addr <= 0x3FFF){
        if(MBC == 1){
            // set lower 5 bits of ROM bank
            ROM_bank &= ~0x1F;
            ROM_bank |= (val & 0x1F);
            if(ROM_bank == 0 || ROM_bank == 0x20 || ROM_bank == 0x40 || ROM_bank == 0x60){
                ROM_bank++;
            }
        }
    }

    // MBC1 mode 0: Upper bit ROM bank select
    // MBC1 mode 1: RAM bank select
    else if(MBC == 1 && 0x4000 <= addr && addr <= 0x5FFF){
        if(MBC_mode){
            RAM_bank = val & 3;
        }else{
            // set bits 5 and 6 of ROM bank
            ROM_bank &= ~(0b11 << 5);
            ROM_bank |= (val & 0b11) << 5;
        }
    }

    // MBC1 mode select
    else if(MBC == 1 && 0x6000 <= addr && addr <= 0x7FFF){
        if(val < 2){
            MBC_mode = val;
        }
    }

    // RAM banks 0-N
    if(0xA000 <= addr && addr <= 0xBFFF){
        external_RAM[0x2000 * RAM_bank + (addr - 0xA000)] = val;
    }

    else{
        gb_memory[addr] = val;
    }
}