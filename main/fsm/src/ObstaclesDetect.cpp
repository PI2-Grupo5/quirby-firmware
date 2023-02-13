#include "ObstaclesDetect.h"

ObstaclesDetect::ObstaclesDetect()
{
    isBlockedLeft = false;
    isBlockedMiddle = false;
    isBlockedRight = false;
}


bool ObstaclesDetect::getBlockedLeft()
{
    if ( distanceR < 10 )
    {
        return true;
    }
    return false;
}

bool ObstaclesDetect::getBlockedMiddle()
{
    if ( distanceM < 10 )
    {
        return true;
    }
    return false;
}

bool ObstaclesDetect::getBlockedRight()
{
    if ( distanceL < 10 )
    {
        return true;
    }
    return false;
}

// TODO FAZER TESTES COM O MODULO HC-SR04
void ObstaclesDetect::run()
{
    // while(1)
    // {
    //     gpio_set_level((gpio_num_t)PIN_IN3, 1);
    //     gpio_set_level((gpio_num_t)PIN_IN3, 1);
    //     gpio_set_level((gpio_num_t)PIN_IN3, 1);
    // }
}

