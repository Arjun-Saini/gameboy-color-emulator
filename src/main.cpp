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

    cpu.mmu.MBC = 1;

    std::cout << "ROM bank: " << int(cpu.mmu.ROM_bank) << std::endl;
    std::cout << "ROM bank mode: " << int(cpu.mmu.MBC_mode) << std::endl;
    std::cout << "RAM bank: " << int(cpu.mmu.RAM_bank) << std::endl << std::endl;

    std::cout << "ROM bank: " << int(cpu.mmu.ROM_bank) << std::endl;
    std::cout << "ROM bank mode: " << int(cpu.mmu.MBC_mode) << std::endl;
    std::cout << "RAM bank: " << int(cpu.mmu.RAM_bank) << std::endl << std::endl;

    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current_time;

//    while(true){
//        current_time = std::chrono::steady_clock::now();
//
//        // TODO SDL event loop and inputs
//
//        // Enters this block once per frame
//        uint8_t frame_count = 0;
//        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count() > FRAME_MS_DELTA){
//            last_time = current_time;
//
//            // Loops through all t-cycles for this frame
//            uint32_t total_cycles = 0;
//            while(total_cycles < cpu.cycles_per_frame){
//                if(cpu.halted || cpu.stopped){
//                    // Fake CPU cycle
//                    cpu.t_cycles++;
//                }else{
//                    uint16_t opcode = cpu.next_opcode();
//                    cpu.decode_opcode(opcode);
//                }
//
//                total_cycles += cpu.t_cycles;
//
//                // Advances PPU and APU to catch up with CPU
//                for(; cpu.t_cycles > 0; cpu.t_cycles--){
//                    if(!cpu.stopped){
//                        ppu.tick();
//                        apu.tick();
//                    }
//                }
//
//                // Delays EI by one instruction
//                if(next_interrupt){
//                    cpu.IME = true;
//                    next_interrupt = false;
//                }
//                if(cpu.enable_interrupt){
//                    next_interrupt = true;
//                    cpu.enable_interrupt = false;
//                }
//
//                cpu.detect_interrupt();
//            }
//
//            if(++frame_count == 60){
//                frame_count = 0;
//                cpu.mmu.update_RTC();
//            }
//        }
//    }

    // TODO save external_RAM
    // TODO save RTC data if MBC3

    return 0;
}
