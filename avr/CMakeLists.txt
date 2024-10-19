# Project name
set(PROJECT robot-avr)

# Generate smallest possible binaries
set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Choose the type of build, options are: Debug, Release, MinSizeRel or RelWithDebInfo" FORCE)

# Min CMake version
cmake_minimum_required(VERSION 3.19)

# Setup arduino-cli
set (CMAW_ARDUINOCLI_BINARY_NOSYSTEM ON CACHE BOOL "When enabled, CMAW will skip searching for arduino-cli on the system")
include("../lib/cmaw/CMAW.cmake")
cmaw_init_config("./cmaw/")
cmaw_update_core_index()
cmaw_install_cores("arduino:avr")

## Enable "unsafe" install (ie. install from git repo) if not enabled
#cmaw_config_get_bool("library.enable_unsafe_install" unsafeInstallFlag)
#if (NOT unsafeInstallFlag)
#    message("setting arduino unsafe install flag")
#    cmaw_config_set("library.enable_unsafe_install" "true")
#endif()
#message("arduino unsafe install flag: ${unsafeInstallFlag}")
#
## Install libraries from git src
#cmaw_install_libraries_git("https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library")

# Setup AVR toolchain
# must be before project() as eg CMAKE_C_COMPILER would be overriden by it
# placing it before allows everything to be setup properly after the first cmake call
include(../lib/avr-cmake/cmake/toolchain/uno.toolchain.cmake)
#include(../lib/avr-cmake/cmake/lto.cmake) # seemingly unsuported by avr-gcc?
include(../lib/avr-cmake/cmake/arduino_core.cmake)
include(../lib/avr-cmake/cmake/arduino_hex.cmake)
include(../lib/avr-cmake/cmake/arduino_upload.cmake)


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

add_executable(${PROJECT} src/main.cpp)

target_sources(${PROJECT} PUBLIC
     ${CMAKE_CURRENT_LIST_DIR}/src/drive.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/motor_controller.cpp
)

#add_executable(${PROJECT} ${FREERTOS_ARDUINO_PATH}/variantHooks.cpp)

target_link_libraries(${PROJECT} PUBLIC ArduinoCore FreeRTOS-Kernel FreeRTOS-Kernel-Heap3)
target_compile_options(${PROJECT} PRIVATE
    "-Wall"
    "-Wextra"
    "-pedantic"
)
arduino_avr_hex(${PROJECT})