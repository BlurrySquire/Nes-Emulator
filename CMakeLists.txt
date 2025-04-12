cmake_minimum_required(VERSION 3.15)

project(NesEmu VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(NesEmu
	source/main.c
	source/memory_bus.c
	source/cpu.c
)

find_package(SDL2 REQUIRED)
target_link_libraries(NesEmu PRIVATE SDL2::SDL2 SDL2::SDL2main)