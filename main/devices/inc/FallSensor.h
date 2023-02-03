#pragma once

#include <iostream>
#include <esp_log.h>
#include <driver/gpio.h>

#include "IRPin.h"

class FallSensor {
public:
    FallSensor();

    void setupPin(gpio_num_t pinSet);
    u_int8_t getDrop();



private:
    u_int8_t isDrop;

};