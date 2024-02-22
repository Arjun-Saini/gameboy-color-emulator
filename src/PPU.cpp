#include "PPU.h"

void PPU::tick() {
    prev_stat_interrupt = stat_interrupt;
    stat_interrupt = false;

    scanline = cycles / 456;
    mmu->write_byte(LY, scanline);

    if(mmu->read_byte(LY) == mmu->read_byte(WY)){
        WY_LY = true;
    }

    // Sets LY == LYC bit in LCD Status register
    mmu->gb_memory[LCD_STATUS] &= ~(1 << 2);
    mmu->gb_memory[LCD_STATUS] |= (scanline == mmu->read_byte(LY_COMPARE)) << 2;

    if((mmu->gb_memory[LCD_STATUS] >> 6) & 1 && scanline == mmu->read_byte(LY_COMPARE)){
        stat_interrupt = true;
    }

    scan_pos = cycles % 456;

    if(scanline < 144){
        if(scan_pos == 0){
            h_blank = false;
            drawing = true;
            LX = 0;
            background_fetch_X = 0;
            background_fetch_stage = 0;
            sprite_fetch_stage = 0;
            discard_pixels = mmu->read_byte(SCX) % 8;
            background_fifo = {};
            sprite_fifo = {};
            sprite_buffer = {};
            draw_background = true;
            window_reset_on_scanline = false;

            if(window_on_scanline){
                window_line_counter++;
                window_on_scanline = false;
            }

            if((mmu->read_byte(LCD_STATUS) >> 5) & 1){
                stat_interrupt = true;
            }
        }

        if(scan_pos < 80){
            mmu->gb_memory[LCD_STATUS] &= ~0b11;
            mmu->gb_memory[LCD_STATUS] |= 0b10;
            OAM_scan();
        }else if(drawing){
            mmu->gb_memory[LCD_STATUS] &= ~0b11;
            mmu->gb_memory[LCD_STATUS] |= 0b11;
            draw_pixels();
        }else{
            mmu->gb_memory[LCD_STATUS] &= ~0b11;
            h_blank = true;
        }
    }else{
        window_line_counter = 0;

        mmu->gb_memory[LCD_STATUS] &= ~0b11;
        mmu->gb_memory[LCD_STATUS] |= 0b01;
        v_blank = true;
    }

    if(scanline == 144){
        cpu->request_interrupt(0);

        if((mmu->read_byte(LCD_STATUS) >> 4) & 1){
            stat_interrupt = true;
        }
    }

    if(++cycles == CYCLES_PER_FRAME){
        v_blank = false;
        cycles = 0;
    }

    if(stat_interrupt && !prev_stat_interrupt){
        cpu->request_interrupt(1);
    }
}

void PPU::OAM_scan() {
    if(scan_pos % 2){
        uint8_t sprite = (scan_pos + 1) / 2;
        uint8_t sprite_X = mmu->read_byte(0xFE00 + sprite * 4 + 1);
        uint8_t sprite_Y = mmu->read_byte(0xFE00 + sprite * 4);
        uint8_t tile_size = ((mmu->read_byte(LCD_CONTROL) >> 2) & 1) * 8 + 8;

        if(sprite_X > 0 && scanline + 16 >= sprite_Y && scanline + 16 < sprite_Y + tile_size && sprite_buffer.size() < 10){
            sprite_buffer.push_back(sprite);
        }
    }
}

void PPU::draw_pixels() {
    background_fetch();
//    sprite_fetch();

    if(!background_fifo.empty() && sprite_fetch_stage == 0){
        Pixel p = background_fifo.front();
        background_fifo.pop();

        if(discard_pixels > 0){
            discard_pixels--;
            return;
        }

        if(!sprite_fifo.empty()){
            Pixel sprite_pixel = sprite_fifo.front();
            sprite_fifo.pop();

            if(!(sprite_pixel.color == 0 || sprite_pixel.background_priority == 1 && p.color != 0)){
                p = sprite_pixel;
            }
        }

        uint8_t color = (mmu->read_byte(p.palette) >> (p.color * 2)) & 3;
        uint32_t hex_color;
        switch (color) {
            case 0:
                hex_color = 0xFFFFFF;
                break;
            case 1:
                hex_color = 0xAAAAAA;
                break;
            case 2:
                hex_color = 0x555555;
                break;
            case 3:
                hex_color = 0x000000;
                break;
        }

        set_hex_color(hex_color);
        SDL_RenderDrawPoint(renderer, LX, scanline);
        LX++;

        if(LX == 160){
            LX = 0;
            drawing = false;

            if((mmu->read_byte(LCD_STATUS) >> 3) & 1){
                stat_interrupt = true;
            }
        }

        if(((mmu->read_byte(LCD_CONTROL) >> 5) & 1) && WY_LY && LX >= mmu->read_byte(WX) - 7){
            background_fetch_stage = 0;
            draw_background = false;

            if(!window_reset_on_scanline){
                background_fetch_X = 0;
                window_reset_on_scanline = true;
            }
        }
    }
}

void PPU::set_hex_color(uint32_t hex_color) {
    SDL_SetRenderDrawColor(renderer, (hex_color & 0xFF0000) >> 16, (hex_color & 0xFF00) >> 8, hex_color & 0xFF, 255);
}

void PPU::background_fetch() {
    switch (background_fetch_stage) {
        case 0: case 2: case 4:
            background_fetch_stage++;
            break;
        case 1:
            // fetch tile ID
            uint16_t tile_addr;
            if(draw_background){
                // fetch background ID
                if((mmu->read_byte(LCD_CONTROL) >> 3) & 1){
                    tile_addr = 0x9C00;
                }else{
                    tile_addr = 0x9800;
                }
                tile_addr += ((background_fetch_X + mmu->read_byte(SCX)) & 0xFF) / 8;
                tile_addr += (((mmu->read_byte(LY) + mmu->read_byte(SCY)) & 0xFF) / 8) << 5;
            }else{
                window_on_scanline = true;

                // fetch window ID
                if((mmu->read_byte(LCD_CONTROL) >> 6) & 1){
                    tile_addr = 0x9C00;
                }else{
                    tile_addr = 0x9800;
                }
                tile_addr += background_fetch_X / 8;
                tile_addr += (window_line_counter / 8) << 5;
            }

            tile_ID = mmu->read_byte(tile_addr);
            background_fetch_stage++;
            break;
        case 3:
            // fetch tile data low
            uint16_t tile_data_low_addr;
            if(mmu->read_byte(LCD_CONTROL) >> 4 & 1){
                // 8000 address mode
                tile_data_low_addr = 0x8000 + tile_ID * 16;
            }else{
                // 8800 address mode
                tile_data_low_addr = 0x9000 + int8_t(tile_ID) * 16;
            }

            if(draw_background){
                tile_data_low_addr += 2 * ((mmu->read_byte(LY) + mmu->read_byte(SCY)) % 8);
            }else{
                tile_data_low_addr += 2 * (window_line_counter % 8);
            }

            tile_data_low = mmu->read_byte(tile_data_low_addr);
            background_fetch_stage++;
            break;
        case 5:
            // fetch tile data high
            uint16_t tile_data_high_addr;
            if(mmu->read_byte(LCD_CONTROL) >> 4 & 1){
                // 8000 address mode
                tile_data_high_addr = 0x8000 + tile_ID * 16 + 1;
            }else{
                // 8800 address mode
                tile_data_high_addr = 0x9000 + int8_t(tile_ID) * 16 + 1;
            }

            if(draw_background){
                tile_data_high_addr += 2 * ((mmu->read_byte(LY) + mmu->read_byte(LY_COMPARE)) % 8);
            }else{
                tile_data_high_addr += 2 * (window_line_counter % 8);
            }
            tile_data_high = mmu->read_byte(tile_data_high_addr);

            // Reset fetcher on first iteration in a scanline
            if(scan_pos == 5){
                background_fetch_stage = 0;
            }else{
                background_fetch_stage++;
            }
            break;
        case 6:
            // create and push pixels to FIFO when it is empty
            if(background_fifo.empty()){
                for(int i = 7; i >= 0; i--){
                    Pixel p = Pixel();
                    if(mmu->read_byte(LCD_CONTROL) & 1){
                        p.color = (((tile_data_high >> i) & 1) << 1) | ((tile_data_low >> i) & 1);
                    }else{
                        p.color = 0;
                    }

                    p.palette = BGP;
                    background_fifo.push(p);
                    background_fetch_X++;
                }
                background_fetch_stage = 0;
            }
            break;
    }
}

void PPU::sprite_fetch() {
    for(int i = 0; i < sprite_buffer.size(); i++){
        uint8_t sprite_X = mmu->read_byte(0xFE00 + sprite_buffer.at(i) * 4 + 1);
        uint8_t sprite_Y = mmu->read_byte(0xFE00 + sprite_buffer.at(i) * 4);
        if(sprite_X <= LX + 8){
            background_fetch_stage = 0;
            switch (sprite_fetch_stage) {
                case 0: case 2: case 4:
                    sprite_fetch_stage++;
                    return;
                case 1:
                    sprite_tile_ID = mmu->read_byte(0xFE00 + sprite_buffer.at(i) * 4 + 2);
                    sprite_flags = mmu->read_byte(0xFE00 + sprite_buffer.at(i) * 4 + 3);
                    sprite_fetch_stage++;
                    return;
                case 3:
                    uint16_t sprite_data_low_addr;
                    sprite_data_low_addr = 0x8000 + sprite_tile_ID * 16;
                    sprite_data_low_addr += 2 * (scanline - sprite_Y % 8);
                    sprite_tile_data_low = mmu->read_byte(sprite_data_low_addr);
                    sprite_fetch_stage++;
                    return;
                case 5:
                    uint16_t sprite_data_high_addr;
                    sprite_data_high_addr = 0x8000 + sprite_tile_ID * 16 + 1;
                    sprite_data_high_addr += 2 * (scanline - sprite_Y % 8);
                    sprite_tile_data_high = mmu->read_byte(sprite_data_high_addr);
                    sprite_fetch_stage++;
                    return;
                case 6:
                    sprite_buffer.erase(sprite_buffer.begin() + i);
                    for(int j = 7 - sprite_fifo.size(); j >= 0; j--){
                        Pixel p = Pixel();
                        p.color = (((sprite_tile_data_high >> j) & 1) << 1) | ((sprite_tile_data_low >> j) & 1);
                        p.palette = OBP0 + ((sprite_flags >> 4) & 1);
                        p.background_priority = (sprite_flags >> 7) & 1;
                        sprite_fifo.push(p);
                    }

                    // TODO perform sprite check again
                    sprite_fetch_stage = 0;
                    return;
            }
        }
    }
}