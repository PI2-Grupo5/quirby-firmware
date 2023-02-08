#include "ObstaclesDetect.h"

ObstaclesDetect::ObstaclesDetect()
{
    valueFSM = 1;
}


bool ObstaclesDetect::getBlockedLeft()
{
    return isBlockedLeft;
}

bool ObstaclesDetect::getBlockedMiddle()
{
    return isBlockedMiddle;
}

bool ObstaclesDetect::getBlockedRight()
{
    return isBlockedRight;
}

// TODO FAZER TESTES COM O MODULO HC-SR04
void ObstaclesDetect::run()
{
    while(1)
    {
        gpio_set_level((gpio_num_t)PIN_IN3, 1);
        gpio_set_level((gpio_num_t)PIN_IN3, 1);
        gpio_set_level((gpio_num_t)PIN_IN3, 1);
    }
}

void ObstaclesDetect::setManualMode(int valueFSM)
{
    this->valueFSM = valueFSM;
}