# Nes-Emulator
A NES emulator project used to learn emulator development. Currently it supports x86-64 Windows and Linux. A MacOS build does not exist because I am unable to test it.

## Implementation Status
Everything is in the works. There is no ppu, just cpu atm. The cpu currently has a few instructions implemented, but not enough to run any programs.  Development is slow because this is a side project.

### Implemented Instructions
- NOP
- LDA
- LDX
- LDY
- JMP

## Building
This project uses the CMake build system. You need to have SDL2 installed alongside an environment variable that points to ``<sdl2_install_dir>/cmake`` called ``SDL2_DIR``.
```sh
mkdir build
cd build
cmake ..
cmake --build .
```
