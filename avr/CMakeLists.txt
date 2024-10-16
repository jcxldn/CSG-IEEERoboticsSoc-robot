# Project name
set (PROJECT robot-avr)

# Min CMake version
cmake_minimum_required(VERSION 3.19)

# Project definitions
# Src: https://github.com/raspberrypi/pico-examples/blob/f800a7e3031e278062639e070c5764adb4a8a0fc/CMakeLists.txt#L6
project(${PROJECT} C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(../lib/avr-cmake/cmake/lto.cmake)
include(../lib/avr-cmake/cmake/toolchain/uno.toolchain.cmake)
include(../lib/avr-cmake/cmake/arduino_core.cmake)
include(../lib/avr-cmake/cmake/arduino_hex.cmake)
include(../lib/avr-cmake/cmake/arduino_upload.cmake)

add_executable(${PROJECT} src/main.cpp)
target_link_libraries(${PROJECT} PUBLIC ArduinoCore)
target_compile_options(${PROJECT} PRIVATE
    "-Wall"
    "-Wextra"
    "-pedantic"
)
arduino_avr_hex(${PROJECT})