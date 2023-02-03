#include "Wheel.h"

Wheel::Wheel() {
    setupPin((gpio_num_t)PIN_IN1);
    setupPin((gpio_num_t)PIN_IN4);
    setupPin((gpio_num_t)PIN_IN3);
    setupPin((gpio_num_t)PIN_IN2);
}

int Wheel::getOddPin() const {
    return oddPin;
}

void Wheel::setOddPin(int oddPin) {
    Wheel::oddPin = oddPin;
}

int Wheel::getEvenPin() const {
    return evenPin;
}

void Wheel::setEvenPin(int evenPin) {
    Wheel::evenPin = evenPin;
}

void Wheel::setupPin(gpio_num_t pinSet) {
    gpio_reset_pin(pinSet);
    gpio_set_direction(pinSet, GPIO_MODE_OUTPUT);
    gpio_pulldown_dis(pinSet);
    gpio_pullup_dis(pinSet);
    
    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_pulldown_dis(GPIO_NUM_2);
    gpio_pullup_dis(GPIO_NUM_2);
}
