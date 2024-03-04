# gameboy color emulator
Emulates the Nintendo Game Boy Color (GBC) which can 
run games using MBC1, MBC2, MBC3 and MBC5 cartridges. 
Maintains T-cycle accuracy across all components to 
match performance and behavior of original hardware.
___
## Building
A `CMakeLists.txt` configuration file is provided for
compilation of the project. The [SDL2 framework](https://github.com/libsdl-org/SDL/releases)
is a required dependency for functional audio and graphics, this project was developed using
SDL2 2.3.0.
___
## File Structure
**main:** Manages high level control flow of program, 
ensures that all components are synchronized.
The [Game Boy pandocs](https://gbdev.io/pandocs/)
contain most of the information necessary to understand
the interactions and timings between each component.

**CPU (Central Processing Unit):** Responsible for the 
fetch-decode-execute cycle that translates cartridge ROM 
data into C++ instructions. Contains internal registers, 
including the program counter and stack pointer which
identify the current state of the program. Services
interrupts when requested. Details about the instruction 
set can be found [here](https://rgbds.gbdev.io/docs/v0.7.0/gbz80.7),
and their binary mappings can be found [here](https://izik1.github.io/gbops/index.html).
[This](https://robertheaton.com/gameboy-doctor/) program 
was used to help debug issues with the instruction set 
implementation.

**MMU (Memory Management Unit):** Controls access to 
internal memory, all ROM and RAM read/write operations 
must pass through this component. Behavior is determined 
by the MBC chip specified by the currently loaded cartridge.
Each MBC chip has a certain ROM and RAM capacity and various 
methods of switching between internal banks of memory.

**PPU (Pixel Processing Unit):** Cycles between
fetch, draw, and idle modes to continuously update the
screen. When the PPU is drawing pixels, it does so at 
a rate of 1 per 4 MHz cycle. For a more detailed explanation 
of the various PPU modes and functionality, consult [these](https://hacktix.github.io/GBEDG/) 
[resources](https://github.com/ISSOtm/pandocs/blob/rendering-internals/src/Rendering_Internals.md).

**APU:** _CURRENTLY NOT IMPLEMENTED_

**Registers:** Contains eight 8-bit registers used by the 
CPU to store data which can also be combined into 4 16-bit
registers. Register F functions as a container for various
flags set by arithmetic and logic instructions.

**Timer:** Internal 4 MHz clock that is exposed as a 16 KHz
clock to other hardware components. Requests interrupts
at various intervals depending on the configuration.

**Pixel:** Used by the PPU as a container for pixel 
attributes.

**Debug:** Can print various formatted subsets of the
system information and internal memory to make the
data more human-readable.