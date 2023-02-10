#include "Movement.h"

Movement::Movement()
{
    valueFSM = 1;
    machineState = 0;
}

bool Movement::stop()
{
    //RODA 1
    gpio_set_level((gpio_num_t)PIN_IN3, 0);
    gpio_set_level((gpio_num_t)PIN_IN4, 0);
    //RODA 2
    gpio_set_level((gpio_num_t)PIN_IN2, 0);
    gpio_set_level((gpio_num_t)PIN_IN1, 0);

    return true;
}

bool Movement::goFront()
{
    //RODA 1
    gpio_set_level((gpio_num_t)PIN_IN3, 1);
    gpio_set_level((gpio_num_t)PIN_IN4, 0);
    //RODA 2
    gpio_set_level((gpio_num_t)PIN_IN2, 0);
    gpio_set_level((gpio_num_t)PIN_IN1, 1);

    return true;
}

bool Movement::goRight()
{
    //RODA 1
    gpio_set_level((gpio_num_t)PIN_IN3, 1);
    gpio_set_level((gpio_num_t)PIN_IN4, 0);
    //RODA 2
    gpio_set_level((gpio_num_t)PIN_IN2, 1);
    gpio_set_level((gpio_num_t)PIN_IN1, 0);

    return true;
}

bool Movement::goDown()
{
    //RODA 1
    gpio_set_level((gpio_num_t)PIN_IN3, 0);
    gpio_set_level((gpio_num_t)PIN_IN4, 1);
    //RODA 2
    gpio_set_level((gpio_num_t)PIN_IN2, 1);
    gpio_set_level((gpio_num_t)PIN_IN1, 0);

    return true;
}

bool Movement::goLeft()
{
    //RODA 1
    gpio_set_level((gpio_num_t)PIN_IN3, 0);
    gpio_set_level((gpio_num_t)PIN_IN4, 1);
    //RODA 2
    gpio_set_level((gpio_num_t)PIN_IN2, 0);
    gpio_set_level((gpio_num_t)PIN_IN1, 1);

    return true;
}



void Movement::run()
{
    switch (valueFSM)
    {
    case 0:
        stop();
        break;
    case 1:
        goFront();
        break;
    case 2:
        goRight();
        break;
    case 3:
        goDown();
        break;
    case 4:
        goLeft();
        break;
    default:
        std::cout << "Error" << '\n';
        break;
    }
}

int Movement::getMode()
{
    return this->valueFSM;
}

void Movement::setManualMode(int valueFSM)
{
    this->valueFSM = valueFSM;
}