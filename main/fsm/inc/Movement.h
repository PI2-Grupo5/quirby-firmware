#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h" 
#include "Wheel.h"
#include "driver/gpio.h"
#include "esp_random.h"
#include <esp_log.h>
#include <string>

class Movement
{
public:

    Wheel iniciarRodas;
    
    Movement();

    bool goFront();
    bool goRight();
    bool goDown();
    bool goLeft();
    bool stop();
    void run();
    int  getMode();
    void setManualMode(int valueFSM);

private:
    int valueFSM;
    int machineState;

};
