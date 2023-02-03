#pragma once

#include <iostream>
#include <esp_log.h>
#include <driver/gpio.h>

#include "WheelPin.h"

class Wheel {
public:
    Wheel();

    void setupPin(gpio_num_t pinSet);

    int getOddPin() const;
    void setOddPin(int oddPin);

    int getEvenPin() const;
    void setEvenPin(int evenPin);

private:
    int oddPin;
    int evenPin;

};