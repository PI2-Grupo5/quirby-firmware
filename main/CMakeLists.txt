set(DEVICES devices/src/UltrasonicSensors.cpp devices/src/Wheel.cpp devices/src/FallSensor.cpp devices/src/PWM.cpp)
set(FSM fsm/src/Movement.cpp)

set(SOURCES quirby-firmware.cpp ${DEVICES} ${FSM})

set(PATH_INC_DIRS enumeration/inc devices/inc fsm/inc )

idf_component_register(SRCS ${SOURCES}
                   INCLUDE_DIRS ${PATH_INC_DIRS}
                   )

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++2a)

