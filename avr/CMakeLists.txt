# Project name
set(PROJECT robot-avr)

# Generate smallest possible binaries
set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Choose the type of build, options are: Debug, Release, MinSizeRel or RelWithDebInfo" FORCE)

# Min CMake version
cmake_minimum_required(VERSION 3.19)

# Project definitions
# Src: https://github.com/raspberrypi/pico-examples/blob/f800a7e3031e278062639e070c5764adb4a8a0fc/CMakeLists.txt#L6
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in FreeRTOS
set(FREERTOS_KERNEL_PATH ../lib/freertos-kernel CACHE PATH "Path to the FreeRTOS Kernel")
set(FREERTOS_ARDUINO_PATH ../lib/freertos-arduino/src CACHE PATH "Path to the FreeRTOS Arduino lib")
set(FREERTOS_PORT_PATH portable/ThirdParty/GCC/ATmega)
include(./freertos-lib.cmake)

include(../lib/avr-cmake/cmake/lto.cmake)
include(../lib/avr-cmake/cmake/toolchain/uno.toolchain.cmake)
include(../lib/avr-cmake/cmake/arduino_core.cmake)
include(../lib/avr-cmake/cmake/arduino_hex.cmake)
include(../lib/avr-cmake/cmake/arduino_upload.cmake)

add_executable(${PROJECT} src/main.cpp)

#add_executable(${PROJECT} ${FREERTOS_ARDUINO_PATH}/variantHooks.cpp)

target_link_libraries(${PROJECT} PUBLIC ArduinoCore FreeRTOS-Kernel FreeRTOS-Kernel-Heap3)
target_compile_options(${PROJECT} PRIVATE
    "-Wall"
    "-Wextra"
    "-pedantic"
)
arduino_avr_hex(${PROJECT})