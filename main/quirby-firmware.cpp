#include <iostream>

#include "IOPin.h"
#include "IRPin.h"
#include "WheelPin.h"
#include "UltrasonicSensors.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"


extern "C" void app_main()
{
        UltrasonicSensors teste(GPIO_NUM_0,GPIO_NUM_3);

}
