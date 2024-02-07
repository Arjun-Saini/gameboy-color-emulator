#include "GameboyCPU.h"

int main(int argc, char* argv[]) {
    GameboyCPU cpu = GameboyCPU();
    cpu.load_ROM(argv[0], "test");

    return 0;
}
