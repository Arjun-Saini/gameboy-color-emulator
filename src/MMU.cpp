#include "MMU.h"

MMU::MMU() {
    // Clear memory
    std::fill(std::begin(gb_memory), std::end(gb_memory), 0);
    std::fill(std::begin(cartridge_memory), std::end(cartridge_memory), 0);
}

// Loads ROM into memory
void MMU::load_ROM(std::string exec_path, std::string file_name) {
    std::filesystem::path fs_exec_path(exec_path);
    std::string gbPath = fs_exec_path.parent_path().parent_path().string() + "/ROMs/" + file_name + ".gb";
    std::string gbcPath = fs_exec_path.parent_path().parent_path().string() + "/ROMs/" + file_name + ".gbc";

    // Open ROM
    std::ifstream ROM;
    ROM = std::ifstream(gbPath, std::ios::binary);
    if(!ROM.is_open()) {
        ROM = std::ifstream(gbcPath, std::ios::binary);
        if(!ROM.is_open()){
            std::cout << "Failed to open ROM" << std::endl;
        }
    }

    ROM.seekg(0, std::ios::end);
    std::streamoff fileSize = ROM.tellg();
    ROM.seekg(0, std::ios::beg);

    // Load ROM into buffer
    char buffer[fileSize];
    ROM.read(buffer, fileSize);

    // Copy buffer into cartridge memory
    for(int i = 0; i < fileSize; i++){
        cartridge_memory[i] = buffer[i];
    }
}

uint8_t MMU::read_byte(uint16_t addr) {
    return gb_memory[addr];
}

void MMU::write_byte(uint16_t addr, uint8_t val) {
    gb_memory[addr] = val;
}