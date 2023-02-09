#pragma once

#include <iostream>
#include <esp_log.h>
#include <driver/gpio.h>

#include "IRPin.h"

class FallSensor {
public:
    FallSensor();

    void setupPin(gpio_num_t pinSet);
    int getDrop();



private:
    int isDrop;

};