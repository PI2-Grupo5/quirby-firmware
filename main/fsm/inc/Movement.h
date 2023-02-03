#pragma once

#include "Wheel.h"
#include "driver/gpio.h"

class Movement
{
public:

    Wheel iniciarRodas;

    Movement();

    bool goFront();
    bool goRight();
    bool goDown();
    bool goLeft();
    void run();
    void setManualMode(int valueFSM);

private:

    int valueFSM;

};