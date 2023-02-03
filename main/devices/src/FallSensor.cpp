#include "FallSensor.h"

FallSensor::FallSensor() {
    setupPin((gpio_num_t)PIN_INFRA);
}

u_int8_t FallSensor::getDrop() {
    return isDrop;
}

void FallSensor::setupPin(gpio_num_t pinSet) {
    gpio_reset_pin(pinSet);
    gpio_set_direction(pinSet, GPIO_MODE_INPUT);
    gpio_pulldown_en(pinSet);
    gpio_pullup_dis(pinSet);
}
