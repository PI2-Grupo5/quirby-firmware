#pragma once

#include "UltrasonicSensors.h"
#include "driver/gpio.h"

class ObstaclesDetect
{
public:

    UltrasonicSensors initUltrasonic;

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
