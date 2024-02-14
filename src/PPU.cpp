#include "PPU.h"

PPU::PPU() {

}

void PPU::tick() {
    scanline = cycles / 456;
    scan_pos = cycles % 456;
    if(scanline < 144){
        if(scan_pos < 80){
            // Mode 2
            h_blank = false;
            OAM_scan();
        }else if(drawing){
            // Mode 3
            draw_pixels();
        }else{
            // Mode 0
            sprite_buffer.clear();
            h_blank = true;
        }
    }else{
        // Mode 1
        v_blank = true;
    }

    if(++cycles == CYCLES_PER_FRAME){
        v_blank = false;
        cycles = 0;
    }
}

void PPU::OAM_scan() {
    if(scan_pos % 2){
        uint8_t sprite = (scan_pos + 1) / 2;
        uint8_t sprite_X = mmu->read_byte(0xFE00 + sprite * 4 + 1);
        uint8_t sprite_Y = mmu->read_byte(0xFE00 + sprite * 4);
        uint8_t tile_size = ((mmu->read_byte(LCD_CONTROL) >> 2) & 1) * 8 + 8;
        uint8_t lY = mmu->read_byte(LY);

        if(sprite_X > 0 && lY + 16 >= sprite_Y && lY + 16 < sprite_Y + tile_size && sprite_buffer.size() < 10){
            sprite_buffer.push_back(sprite);
        }
    }
}

// TODO toggle drawing when done
void PPU::draw_pixels() {

}