#include "GameboyCPU.h"

GameboyCPU::GameboyCPU() {
    // Clear memory
    std::fill(std::begin(gb_memory), std::end(gb_memory), 0);
    std::fill(std::begin(cartridge_memory), std::end(cartridge_memory), 0);

    registers = Registers();

    // Initialize values
    program_counter = 0x100;
    stack_pointer = 0xFFFE;
    registers.set_reg_AF(0x01B0);
    registers.set_reg_BC(0x0013);
    registers.set_reg_DE(0x00D8);
    registers.set_reg_HL(0x014D);
    gb_memory[0xFF05] = 0x00;
    gb_memory[0xFF06] = 0x00;
    gb_memory[0xFF07] = 0x00;
    gb_memory[0xFF10] = 0x80;
    gb_memory[0xFF11] = 0xBF;
    gb_memory[0xFF12] = 0xF3;
    gb_memory[0xFF14] = 0xBF;
    gb_memory[0xFF16] = 0x3F;
    gb_memory[0xFF17] = 0x00;
    gb_memory[0xFF19] = 0xBF;
    gb_memory[0xFF1A] = 0x7F;
    gb_memory[0xFF1B] = 0xFF;
    gb_memory[0xFF1C] = 0x9F;
    gb_memory[0xFF1E] = 0xBF;
    gb_memory[0xFF20] = 0xFF;
    gb_memory[0xFF21] = 0x00;
    gb_memory[0xFF22] = 0x00;
    gb_memory[0xFF23] = 0xBF;
    gb_memory[0xFF24] = 0x77;
    gb_memory[0xFF25] = 0xF3;
    gb_memory[0xFF26] = 0xF1;
    gb_memory[0xFF40] = 0x91;
    gb_memory[0xFF42] = 0x00;
    gb_memory[0xFF43] = 0x00;
    gb_memory[0xFF45] = 0x00;
    gb_memory[0xFF47] = 0xFC;
    gb_memory[0xFF48] = 0xFF;
    gb_memory[0xFF49] = 0xFF;
    gb_memory[0xFF4A] = 0x00;
    gb_memory[0xFF4B] = 0x00;
    gb_memory[0xFFFF] = 0x00;
}

// Loads ROM into memory
void GameboyCPU::load_ROM(std::string exec_path, std::string file_name) {
    std::filesystem::path fs_exec_path(exec_path);

    // TODO Check if gb extension is needed
    std::string fullPath = fs_exec_path.parent_path().parent_path().string() + "/ROMs/" + file_name + ".gbc";

    // Open ROM
    std::ifstream ROM(fullPath, std::ios::binary);
    if(!ROM.is_open()) {
        std::cout << "Failed to read cartridge" << std::endl;
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