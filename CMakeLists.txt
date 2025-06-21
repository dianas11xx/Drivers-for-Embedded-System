cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(DriverLab C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(level libraries/LIS3DH.h libraries/LIS3DH.cpp libraries/NeoMatrix.h libraries/NeoMatrix.cpp src/level.cpp)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/libraries/generated)
# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(level ${CMAKE_CURRENT_LIST_DIR}/libraries/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/libraries/generated)
target_link_libraries(level PRIVATE pico_stdlib hardware_pio hardware_i2c)

pico_enable_stdio_usb(level 1)
pico_add_extra_outputs(level)