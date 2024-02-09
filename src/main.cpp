#include "CPU.h"
#include "MMU.h"
#include "PPU.h"
#include "APU.h"
#include "Debug.h"
#include <chrono>

#define FRAME_MS_DELTA 16

int main(int argc, char* argv[]) {
    CPU cpu = CPU();
    PPU ppu = PPU();
    APU apu = APU();
    Debug db = Debug(&cpu);

    cpu.mmu.load_ROM(argv[0], "test");


    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current_time;

    while(false){
        current_time = std::chrono::steady_clock::now();

        // TODO SDL event loop and inputs

        // Enters this block once per frame
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count() > FRAME_MS_DELTA){
            last_time = current_time;

            // Loops through all t-cycles for this frame
            uint32_t total_cycles = 0;
            while(total_cycles < cpu.cycles_per_frame){
                uint16_t opcode = cpu.next_opcode();
                cpu.decode_opcode(opcode);
                total_cycles += cpu.t_cycles;

                // Advances PPU and APU to catch up with CPU
                for(int i = 0; i < cpu.t_cycles; i++){
                    ppu.tick();
                    apu.tick();
                }
            }
        }
    }

    return 0;
}
