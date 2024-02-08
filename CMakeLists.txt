cmake_minimum_required(VERSION 3.27)
project(gameboy_color_emulator)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(gameboy_color_emulator src/main.cpp
        src/CPU.cpp
        src/CPU.h
        src/Registers.cpp
        src/Registers.h
        src/Debug.cpp
        src/Debug.h
        src/PPU.cpp
        src/PPU.h
        src/APU.cpp
        src/APU.h
        src/MMU.cpp
        src/MMU.h)
target_link_libraries(gameboy_color_emulator PRIVATE SDL2::SDL2)