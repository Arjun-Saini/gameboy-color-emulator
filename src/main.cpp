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

    bool next_interrupt = false;
    cpu.mmu.load_ROM(argv[0], "test");

    db.print_registers();
    cpu.stack_pointer = 0xF;
    cpu.LD_HL_SPi8();
    db.print_registers();

    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current_time;

    while(true){
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
                for(; cpu.t_cycles > 0; cpu.t_cycles--){
                    ppu.tick();
                    apu.tick();
                }

                if(next_interrupt){
                    cpu.IME = true;
                    next_interrupt = false;
                }
                if(cpu.enable_interrupt){
                    next_interrupt = true;
                    cpu.enable_interrupt = false;
                }
            }
        }
    }

    return 0;
}
