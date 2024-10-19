set(LIBPRINTF_PATH ${CMAW_OPTIONAL_LIB_PATH}/LibPrintf/src)

add_library(LibPrintf STATIC
    ${LIBPRINTF_PATH}/LibPrintf.cpp
)

target_link_libraries(LibPrintf PUBLIC ArduinoFlags ArduinoCore)
target_compile_features(LibPrintf PUBLIC cxx_std_11 c_std_11)
target_include_directories(LibPrintf PUBLIC
    ${LIBPRINTF_PATH}/
)
