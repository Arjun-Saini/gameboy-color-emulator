#include "PPU.h"

void PPU::tick() {
    scanline = cycles / 456;
    mmu->write_byte(LY, scanline);

    // Sets LY == LYC bit in LCD Status register
    uint8_t lcd_status = mmu->read_byte(LCD_STATUS);
    lcd_status &= ~(1 << 2);
    lcd_status |= (scanline == mmu->read_byte(LY_COMPARE)) << 2;
    mmu->write_byte(LCD_STATUS, lcd_status);

    scan_pos = cycles % 456;

    if(scanline < 144){
        if(scan_pos == 0){
            h_blank = false;
            drawing = true;
            background_fifo_X = 0;
            if(window_on_scanline){
                window_line_counter++;
                window_on_scanline = false;
            }

            if((mmu->read_byte(LCD_STATUS) >> 5) & 1){
                cpu->request_interrupt(1);
            }
        }

        if(scan_pos < 80){
            uint8_t stat = mmu->read_byte(LCD_STATUS);
            stat &= ~0b11;
            stat |= 2;
            mmu->write_byte(LCD_STATUS, stat);
            OAM_scan();
        }else if(drawing){
            uint8_t stat = mmu->read_byte(LCD_STATUS);
            stat &= ~0b11;
            stat |= 3;
            mmu->write_byte(LCD_STATUS, stat);
            draw_pixels();
        }else{
            uint8_t stat = mmu->read_byte(LCD_STATUS);
            stat &= ~0b11;
            stat |= 0;
            mmu->write_byte(LCD_STATUS, stat);
            if((stat >> 3) & 1){
                cpu->request_interrupt(1);
            }
            sprite_buffer.clear();
            h_blank = true;
        }
    }else{
        uint8_t stat = mmu->read_byte(LCD_STATUS);
        stat &= ~0b11;
        stat |= 1;
        mmu->write_byte(LCD_STATUS, stat);
        if((stat >> 4) & 1){
            cpu->request_interrupt(1);
        }
        window_line_counter = 0;
        v_blank = true;
    }

    if(scanline == 144){
        cpu->request_interrupt(0);
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

void PPU::draw_pixels() {
    background_fetch();
    if(!background_fifo.empty()){
        Pixel p = background_fifo.front();
        background_fifo.pop();

        uint8_t color = (mmu->read_byte(BGP) >> (p.color * 2)) & 3;
        uint32_t hex_color;
        switch (color) {
            case 0:
                hex_color = 0xFFFFFF;
                break;
            case 1:
                hex_color = 0x808080;
                break;
            case 2:
                hex_color = 0x808080;
                break;
            case 3:
                hex_color = 0x000000;
                std::cout << "BLACK" << std::endl;
                break;
        }

        set_hex_color(hex_color);
        SDL_RenderDrawPoint(renderer, pixel_x_counter, scanline);

        pixel_x_counter++;

        if(pixel_x_counter == 160){
            pixel_x_counter = 0;
            drawing = false;
        }
    }
}

void PPU::set_hex_color(uint32_t hex_color) {
    SDL_SetRenderDrawColor(renderer, (hex_color & 0xFF0000) >> 16, (hex_color & 0xFF00) >> 8, hex_color & 0xFF, 255);
}

void PPU::background_fetch() {
    switch (background_fifo_stage) {
        case 0: case 2: case 4: case 6:
            background_fifo_stage++;
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
                tile_addr += ((background_fifo_X + mmu->read_byte(SCX)) & 0xFF) / 8;
                tile_addr += (((mmu->read_byte(LY) + mmu->read_byte(SCY)) & 0xFF) / 8) << 5;
            }else{
                // fetch window ID
                if((mmu->read_byte(LCD_CONTROL) >> 6) & 1){
                    tile_addr = 0x9C00;
                }else{
                    tile_addr = 0x9800;
                }
                tile_addr += background_fifo_X / 8;
                tile_addr += (window_line_counter / 8) << 5;
            }

            tile_ID = mmu->read_byte(tile_addr);
            background_fifo_stage++;
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
            background_fifo_stage++;
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
                background_fifo_stage = 0;
            }else{
                background_fifo_stage++;
            }
            break;
        case 7:
            // create and push pixels to FIFO when it is empty
            if(background_fifo.empty()){
                for(int i = 7; i >= 0; i--){
                    Pixel p = Pixel();
                    p.color = (((tile_data_high >> i) & 1) << 1) | ((tile_data_low >> i) & 1);
                    p.palette = BGP;
                    background_fifo.push(p);
                }
                background_fifo_stage = 0;
                background_fifo_X++;
            }
            break;
    }
}

void PPU::sprite_fetch() {

}