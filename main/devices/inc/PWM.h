#pragma once

#include <iostream>
#include <esp_log.h>
#include <driver/gpio.h>

#include "PWMPin.h"

class PWM
{
public:
    PWM();

    void setupPin(gpio_num_t pinSet);

    int getLevel();
    void setLevel( int level );

    u_int8_t getActive();



private:
    u_int8_t isActive;
    int level;

};