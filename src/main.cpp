#include "CPU.h"
#include "MMU.h"
#include "PPU.h"
#include "APU.h"
#include "Debug.h"

int main(int argc, char* argv[]) {
    CPU cpu = CPU();
    PPU ppu = PPU();
    APU apu = APU();
    Debug db = Debug();



    cpu.mmu.load_ROM(argv[0], "test");
    std::cout << cpu.cycles_per_frame;

    return 0;
}
