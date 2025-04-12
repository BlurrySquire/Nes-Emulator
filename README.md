# Nes-Emulator
A NES emulator project used to learn emulator development. Currently it supports x86-64 Windows and Linux. The debug version includes additional printing which can help debug problems.

## Implementation Status
- All official CPU opcodes are implemented.
- Memory-mapped I/O is in the works.
- Cartridge/rom parsing is also in the works.

## Building
This project uses the CMake build system. If you would rather not build manually you can download the latest GitHub CI build. You can build in debug mode using ``-DCMAKE_BUILD_TYPE=Debug``, and build in release using ``-DCMAKE_BUILD_TYPE=Release``.
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```