cmake_minimum_required(VERSION 3.15)

project(NesEmu VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NesEmu
	source/main.cpp
	source/memory_bus.cpp
	source/cpu.cpp
)

find_package(SDL3 REQUIRED)
target_link_libraries(NesEmu PRIVATE SDL3::SDL3)

find_package(cJSON REQUIRED)
target_link_libraries(NesEmu PRIVATE cjson)