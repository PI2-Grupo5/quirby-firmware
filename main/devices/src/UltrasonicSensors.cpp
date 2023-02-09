#include "UltrasonicSensors.h"

UltrasonicSensors::UltrasonicSensors() {
    setupPin((gpio_num_t)PIN_ECHO_R);
    setupPin((gpio_num_t)PIN_TRIP_R);
    setupPin((gpio_num_t)PIN_ECHO_M);
    setupPin((gpio_num_t)PIN_TRIP_M);
    setupPin((gpio_num_t)PIN_ECHO_L);
    setupPin((gpio_num_t)PIN_TRIP_L);
}

void UltrasonicSensors::setupPin(gpio_num_t pinSet) {
    gpio_reset_pin(pinSet);
    gpio_set_direction(pinSet, GPIO_MODE_INPUT);
    gpio_pulldown_en(pinSet);
    gpio_pullup_dis(pinSet);

}

int UltrasonicSensors::getTrip() const {
    return trip;
}

void UltrasonicSensors::setTrip(int trip) {
    UltrasonicSensors::trip = trip;
}

int UltrasonicSensors::getEcho() const {
    return echo;
}

void UltrasonicSensors::setEcho(int echo) {
    UltrasonicSensors::echo = echo;
}


