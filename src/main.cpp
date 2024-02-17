#include "CPU.h"
#include "MMU.h"
#include "PPU.h"
#include "APU.h"
#include "Timer.h"
#include "Debug.h"
#include "Serial.h"
#include <chrono>

#define FRAME_MS_DELTA 17
#define TIMER_INTERRUPT 2

#define GRAPHICS_SCALE 2

#define DEBUG_ROM_BANK_0 0
#define DEBUG_ROM_BANK_N 1
#define DEBUG_HRAM 2
#define DEBUG_VRAM 3
#define DEBUG_OAM 4
#define DEBUG_EVERYTHING 5

int main(int argc, char* argv[]) {
    CPU cpu = CPU();
    PPU ppu = PPU();
    APU apu = APU();
    Timer timer = Timer();
    Serial serial = Serial();
    Debug db = Debug(&cpu);

    cpu.mmu.timer = &timer;
    timer.mmu = &cpu.mmu;
    ppu.mmu = &cpu.mmu;
    ppu.cpu = &cpu;
    serial.mmu = &cpu.mmu;
    serial.cpu = &cpu;

    // Initialize graphics
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(160 * GRAPHICS_SCALE, 144 * GRAPHICS_SCALE, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, GRAPHICS_SCALE, GRAPHICS_SCALE);

    ppu.renderer = renderer;

    bool next_interrupt = false;
    cpu.mmu.load_ROM(argv[0], "game-boy-test-roms-v6.0/blargg/cpu_instrs/cpu_instrs");

    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current_time;

    int instructions = 1;
    bool is_running = true;
    while(is_running){
        current_time = std::chrono::steady_clock::now();

        // TODO process key inputs
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                is_running = false;
            }else if(e.type == SDL_KEYDOWN){

            }else if(e.type == SDL_KEYUP){

            }
        }

        // Enters this block once per frame
        uint8_t frame_count = 0;
        uint8_t TMA_reload_count = 0;
        bool log = false;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count() > FRAME_MS_DELTA){
            last_time = current_time;

//            if(cpu.program_counter == 0xc303){
//                log = true;
//            }
//
//            if(log){
//                db.print_registers();
//                db.print_info();
//                std::cout << "___________________" << std::endl;
//            }

            // Loops through all t-cycles for this frame
            uint32_t total_cycles = 0;
            while(total_cycles < cpu.cycles_per_frame){
                if(cpu.halted || cpu.stopped){
                    // Fake CPU cycle
                    cpu.t_cycles++;
                }else{
//                    db.doctor_log();

                    uint16_t opcode = cpu.next_opcode();
                    cpu.decode_opcode(opcode);
                    instructions++;
                }

                total_cycles += cpu.t_cycles;

                // Advances peripheral components to catch up with CPU
                for(; cpu.t_cycles > 0; cpu.t_cycles--){
                    if(!cpu.stopped){
//                        ppu.tick();
                        apu.tick();

                        if(TMA_reload_count > 0){
                            TMA_reload_count--;
                            if(TMA_reload_count == 0 && timer.TMA_reload){
                                cpu.mmu.gb_memory[TIMA_addr] = cpu.mmu.gb_memory[TMA_addr];
                                timer.TMA_reload = false;
                                cpu.request_interrupt(TIMER_INTERRUPT);
                            }
                        }

                        if(TMA_reload_count == 0){
                            timer.tick();
                        }

                        if(timer.TMA_reload && TMA_reload_count == 0) {
                            TMA_reload_count = 4;
                        }
                    }
                }

                // Delays EI by one instruction
                if(next_interrupt){
                    cpu.IME = true;
                    next_interrupt = false;
                }
                if(cpu.enable_interrupt){
                    next_interrupt = true;
                    cpu.enable_interrupt = false;
                }

                cpu.detect_interrupt();
            }

            if(cpu.mmu.MBC == 3 && ++frame_count >= 60){
                frame_count = 0;
                cpu.mmu.update_RTC();
            }

            SDL_RenderPresent(renderer);
        }
    }

    // TODO save external_RAM
    // TODO save RTC data if MBC3

    return 0;
}
