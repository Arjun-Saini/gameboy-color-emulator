#ifndef GAMEBOY_COLOR_EMULATOR_PIXEL_H
#define GAMEBOY_COLOR_EMULATOR_PIXEL_H

#import <cstdint>

class Pixel {
public:
    uint8_t color;
    uint16_t palette;

    // only used by sprites in CGB
    uint8_t sprite_priority;

    // only used by sprites
    uint8_t background_priority;

};


#endif //GAMEBOY_COLOR_EMULATOR_PIXEL_H
