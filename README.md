# Nes-Emulator
A NES emulator project used to learn emulator development. Currently it supports x86-64 Windows and Linux. A MacOS build does not exist because I am unable to test it.

## Implementation Status
Everything is in the works. There is no ppu, just cpu atm. The cpu currently has a few instructions implemented, but not enough to run any programs.  Development is slow because this is a side project.

### Implemented Instructions
The instruction groups are referenced from nesdev wiki's instruction reference.
- All access instructions
- All transfer instructions
- All arithmetic instructions
- All shift instructions
- No bitwise instructions
- No compare instructions
- No branch instructions
- Some jump instructions
- All stack instructions
- All flag instructions
- All other instructions

## Building
This project uses the CMake build system. You need to have SDL2 installed alongside an environment variable that points to ``<sdl2_install_dir>/cmake`` called ``SDL2_DIR``.
```sh
mkdir build
cd build
cmake ..
cmake --build .
```
