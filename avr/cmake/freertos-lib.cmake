# Based on FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/library.cmake
# <https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/a49c35b5dc0f1f521eef3ef993d401af7f26f439/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake>

# Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
#
# SPDX-License-Identifier: BSD-3-Clause

add_library(FreeRTOS-Kernel-Core INTERFACE)
target_sources(FreeRTOS-Kernel-Core INTERFACE
        # croutine.c not present in freertos-arduino
        ${FREERTOS_ARDUINO_PATH}/event_groups.c
        ${FREERTOS_ARDUINO_PATH}/list.c
        ${FREERTOS_ARDUINO_PATH}/queue.c
        ${FREERTOS_ARDUINO_PATH}/stream_buffer.c
        ${FREERTOS_ARDUINO_PATH}/tasks.c
        ${FREERTOS_ARDUINO_PATH}/timers.c
        )

target_include_directories(FreeRTOS-Kernel-Core INTERFACE ${FREERTOS_ARDUINO_PATH})

add_library(FreeRTOS-Kernel INTERFACE)
target_sources(FreeRTOS-Kernel INTERFACE
        ${FREERTOS_ARDUINO_PATH}/port.c
        ${FREERTOS_ARDUINO_PATH}/variantHooks.cpp
)

add_library(FreeRTOS-Config INTERFACE ${FREERTOS_ARDUINO_PATH}/FreeRTOSConfig.h  ${FREERTOS_ARDUINO_PATH}/Arduino_FreeRTOS.h)
target_include_directories(FreeRTOS-Config INTERFACE ${FREERTOS_ARDUINO_PATH})

target_include_directories(FreeRTOS-Kernel INTERFACE
        ${FREERTOS_ARDUINO_PATH}/include
        ${FREERTOS_CONFIG_FILE_DIRECTORY})

target_link_libraries(FreeRTOS-Kernel INTERFACE
        FreeRTOS-Config
        FreeRTOS-Kernel-Core
)

add_library(FreeRTOS-Kernel-Heap3 INTERFACE)
target_sources(FreeRTOS-Kernel-Heap3 INTERFACE ${FREERTOS_ARDUINO_PATH}/heap_3.c)
target_link_libraries(FreeRTOS-Kernel-Heap3 INTERFACE FreeRTOS-Kernel)