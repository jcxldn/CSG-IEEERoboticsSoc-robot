set(MPU6050_PATH ${CMAW_OPTIONAL_LIB_PATH}/MPU6050/src)

add_library(MPU6050 STATIC
    ${MPU6050_PATH}/I2Cdev.cpp
    ${MPU6050_PATH}/MPU6050.cpp
    ${MPU6050_PATH}/MPU6050_6Axis_MotionApps20.cpp
    ${MPU6050_PATH}/MPU6050_6Axis_MotionApps612.cpp
    ${MPU6050_PATH}/MPU6050_9Axis_MotionApps41.cpp
)

target_link_libraries(MPU6050 PUBLIC ArduinoFlags ArduinoCore ArduinoWire)
target_compile_features(MPU6050 PUBLIC cxx_std_11 c_std_11)
target_include_directories(MPU6050 PUBLIC
    ${MPU6050_PATH}/
)
