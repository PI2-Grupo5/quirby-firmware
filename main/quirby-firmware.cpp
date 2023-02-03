#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "UltrasonicSensors.h"
#include "Wheel.h"
#include "FallSensor.h"
#include "PWM.h"
#include <inttypes.h>


extern "C" void app_main()
{
        UltrasonicSensors teste;
        Wheel teste2;
        FallSensor teste3;
        PWM teste4;

        int sim;

        while (1)
        {
                gpio_set_level(GPIO_NUM_2, 1);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                gpio_set_level(GPIO_NUM_2, 0);
                vTaskDelay(1000 / portTICK_PERIOD_MS);

                sim = gpio_get_level(GPIO_NUM_35);
                std::cout << "Estevao ta PUTO: " << sim << '\n';
        }

}
