set(DEVICES devices/src/UltrasonicSensors.cpp devices/src/Wheel.cpp devices/src/FallSensor.cpp devices/src/PWM.cpp)
set(FSM fsm/src/Movement.cpp fsm/src/ObstaclesDetect.cpp)
set(UTILS utils/src/ultrasonic.c)
set(COMMS communication/src/wifi.cpp communication/src/Http.cpp communication/src/http_client.cpp)

set(SOURCES quirby-firmware.cpp ${UTILS} ${DEVICES} ${FSM} ${COMMS})

set(PATH_INC_DIRS enumeration/inc devices/inc fsm/inc utils/inc communication/inc)

idf_component_register(SRCS ${SOURCES}
                   INCLUDE_DIRS ${PATH_INC_DIRS}
                   )

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++2a)

