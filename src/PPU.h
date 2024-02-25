#ifndef GAMEBOY_COLOR_EMULATOR_PPU_H
#define GAMEBOY_COLOR_EMULATOR_PPU_H

#include "SDL.h"
#include <queue>
#include <iostream>
#include <vector>
#include "MMU.h"
#include "CPU.h"
#include "Pixel.h"

#define OAM_START_ADDRESS 0xFE00

#define LCD_CONTROL 0xFF40
#define LCD_STATUS 0xFF41
#define SCY 0xFF42
#define SCX 0xFF43
#define LY 0xFF44
#define LY_COMPARE 0xFF45
#define DMA 0xFF46

#define WY 0xFF4A
#define WX 0xFF4B

// used only in non CGB mode
#define BGP 0xFF47
#define OBP0 0xFF48
#define OBP1 0xFF49

#define CYCLES_PER_FRAME 70224

class PPU {
public:
    MMU* mmu;
    CPU* cpu;
    SDL_Renderer* renderer;

    uint32_t cycles = 0;
    uint8_t scanline = 0;
    uint16_t scan_pos = 0;

    uint8_t tile_ID;
    uint8_t tile_data_low;
    uint8_t tile_data_high;

    uint8_t window_line_counter = 0;
    uint8_t background_fetch_X = 0;
    uint8_t discard_pixels = 0;

    uint8_t sprite_tile_ID;
    uint8_t sprite_flags;
    uint8_t sprite_tile_data_low;
    uint8_t sprite_tile_data_high;

    bool drawing = true;
    bool h_blank = false;
    bool v_blank = false;
    bool draw_background = true;
    bool window_on_scanline = false;
    bool WY_LY = false;
    bool prev_stat_interrupt = false;
    bool stat_interrupt = false;
    bool window_reset_on_scanline = false;

    bool log = false;

    std::queue<Pixel> background_fifo;
    uint8_t background_fetch_stage = 0;
    uint8_t LX = 0;

    std::queue<Pixel> sprite_fifo;
    std::vector<uint8_t> sprite_buffer;
    uint8_t sprite_fetch_stage = 0;

    void tick();
    void OAM_scan();
    void draw_pixels();
    void set_hex_color(uint32_t hex_color);
    void background_fetch();
    void sprite_fetch();
};


#endif //GAMEBOY_COLOR_EMULATOR_PPU_H
