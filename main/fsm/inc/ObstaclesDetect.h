#pragma once

#include "UltrasonicSensor.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"

class ObstaclesDetect
{
public:

    UltrasonicSensor initUltrasonicSetup;

    ObstaclesDetect();

    bool getBlockedLeft();
    bool getBlockedMiddle();
    bool getBlockedRight();
    void run();

private:

    bool isBlockedLeft;
    bool isBlockedMiddle;
    bool isBlockedRight;


};
