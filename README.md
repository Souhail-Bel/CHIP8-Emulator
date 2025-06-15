
# CHIP8 Emulator
This is a simple CHIP8 emulator made in C.
## Running the emulator
Simply `make`.
If you want to enable debug mode, simply `make debug=1`.

Afterwards, you can either directly run the emulator or specify a ROM to load as an argument.

    ./bin/chip8emu [PATH_TO_ROM]

## Chip-specific features
* 4KB Memory (Program starts at 0x200)
* 2-byte program counter
* 2-byte I register
* 16 1-byte V registers
* 2-byte stack array
* Sound and delay timers
* 64x32 display
* Hex keyboard

## Code-specific features
* Debug print triggered by **DEBUG_MODE** flag on compilation
* Multiple possible message outputs: Error, Warning, Information

The **ROM** is read through the argument passed during execution, though if none is given, it reverts back to the test featured on https://github.com/corax89/chip8-test-rom.

A **fetch-execute** cycle is included with the speed under the hypothesis that the microprocessor runs at 700Hz, relying on `usleep(TIME IN MICROSECONDS)` from `unistd.h` library.

The OP code execution is done by referring to a LUT based on function pointers.
Having each OP code be a function is useful for debugging.

For this version, the drawing is directly done on the terminal. Future version will have the draw in an SDL2 context as well as add sound.

## Useful Resources
[CHIP-8 - Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
[Tutorials - Interpreting CPU Instructions via an Array of Function Pointers](https://multigesture.net/wp-content/uploads/mirror/zenogais/FunctionPointers.htm)
[Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0)
[kripod/chip8-roms: ROMs for CHIP-8.](https://github.com/kripod/chip8-roms#)
[JohnEarnest/Octo: A Chip8 IDE](https://github.com/JohnEarnest/Octo)
