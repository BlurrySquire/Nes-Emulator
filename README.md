# Nes-Emulator
A NES emulator project used to learn emulator development. Currently it supports x86-64 Windows and Linux. The debug version includes additional printing which can help debug problems. My main source of documentation for this project is the [Nesdev Wiki](https://www.nesdev.org/wiki/Nesdev_Wiki).

## Implementation Status
- All official CPU opcodes are implemented (119/151 tests passing).
- Memory-mapped I/O is in the works.
- Cartridge/rom parsing is also in the works.

## Building
This project uses the CMake build system. If you would rather not build manually you can download the latest GitHub CI build. You can build in debug mode using ``-DCMAKE_BUILD_TYPE=Debug``, and build in release using ``-DCMAKE_BUILD_TYPE=Release``.

### Dependencies
- [SDL3](https://github.com/libsdl-org/SDL) ([V3.2.10](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.10) preferably)
- [cJSON](https://github.com/DaveGamble/cJSON)

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Test Mode
To use test mode run the emulator with the following command line arguments ``--single-step-test <path/to/opcode/test.json>``. The only test format supported is the tests in the [SingleStepTests](https://github.com/SingleStepTests/65x02/tree/main/nes6502) repo.