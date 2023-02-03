#pragma once

#include <iostream>
#include <esp_log.h>
#include <driver/gpio.h>

#include "UltrasonicPin.h"

class UltrasonicSensors {
public:
    int x = 33;
    UltrasonicSensors(gpio_num_t trip, gpio_num_t echo);

    void setupPins(gpio_num_t pinSet);

    int getTrip() const;
    void setTrip(int trip);

    int getEcho() const;
    void setEcho(int echo);

private:
    int trip;
    int echo;

};