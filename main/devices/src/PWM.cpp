#include "PWM.h"

PWM::PWM() {
    setupPin((gpio_num_t)PIN_SIGNAL);
}

u_int8_t PWM::getActive()
{
    return isActive;
}

int PWM::getLevel()
{
    return level;
}

void PWM::setLevel( int level )
{
    this->level = level;
}

void PWM::setupPin(gpio_num_t pinSet) {
    gpio_reset_pin(pinSet);
    gpio_set_direction(pinSet, GPIO_MODE_OUTPUT);
    gpio_pulldown_dis(pinSet);
    gpio_pullup_dis(pinSet);
}
