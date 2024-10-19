set(FASTLED_PATH ${CMAW_OPTIONAL_LIB_PATH}/FastLED/src)

add_library(FastLED STATIC
    ${FASTLED_PATH}/FastLED.cpp
    ${FASTLED_PATH}/bitswap.cpp
    ${FASTLED_PATH}/colorpalettes.cpp
    ${FASTLED_PATH}/colorutils.cpp
    ${FASTLED_PATH}/five_bit_hd_gamma.cpp
    ${FASTLED_PATH}/hsv2rgb.cpp
    ${FASTLED_PATH}/lib8tion.cpp
    ${FASTLED_PATH}/noise.cpp
    ${FASTLED_PATH}/platforms.cpp
    ${FASTLED_PATH}/power_mgt.cpp
    ${FASTLED_PATH}/rgbw.cpp
    ${FASTLED_PATH}/simplex.cpp
    ${FASTLED_PATH}/stub_main.cpp
    ${FASTLED_PATH}/transpose8x1_noinline.cpp
    ${FASTLED_PATH}/wiring.cpp
)

target_link_libraries(FastLED PUBLIC ArduinoFlags ArduinoCore)
target_compile_features(FastLED PUBLIC cxx_std_11 c_std_11)
target_include_directories(FastLED PUBLIC
    ${FASTLED_PATH}/
)
