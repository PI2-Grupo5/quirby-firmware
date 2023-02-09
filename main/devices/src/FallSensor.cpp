#include "FallSensor.h"

FallSensor::FallSensor() {
    setupPin((gpio_num_t)PIN_INFRA);
}

int FallSensor::getDrop() {
    this->isDrop = gpio_get_level((gpio_num_t)PIN_INFRA);
    return isDrop;
}

void FallSensor::setupPin(gpio_num_t pinSet) {
    gpio_reset_pin(pinSet);
    gpio_set_direction(pinSet, GPIO_MODE_INPUT);
    gpio_pulldown_en(pinSet);
    gpio_pullup_dis(pinSet);
    gpio_set_intr_type(pinSet, GPIO_INTR_POSEDGE);
    gpio_set_intr_type(pinSet, GPIO_INTR_POSEDGE);


}
