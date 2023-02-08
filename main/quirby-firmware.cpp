#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h" 
// #include "driver/pulse_cnt.h"
#include "nvs_flash.h"

#include "UltrasonicSensors.h"
#include "Wheel.h"
#include "FallSensor.h"
#include "PWM.h"
#include "Movement.h"
#include "ultrasonic.h"
#include "wifi.h"
#include "Http.h"


#define MAX_DISTANCE_CM 500 // 5m max

#define CMD_START 100
#define CMD_STOP 200
#define CMD_MEASURE 300
#define CMD_CLEAR 400

#define GPIO_TRIGGER 23
#define GPIO_ECHO 35

static const char *TAG = "ULTRASONIC";

typedef struct {
	uint16_t command;
	uint32_t distance;
	TaskHandle_t taskHandle;
} CMD_t;

void ultrasonic(void *pvParamters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	CMD_t cmdBuf;
	cmdBuf.command = CMD_MEASURE;
	cmdBuf.taskHandle = xTaskGetCurrentTaskHandle();

	ultrasonic_sensor_t sensor = {
		.trigger_pin = (gpio_num_t)GPIO_TRIGGER,
		.echo_pin = (gpio_num_t)GPIO_ECHO
	};

	ultrasonic_init(&sensor);

	while (true) {
		uint32_t distance;
		esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance);
		if (res != ESP_OK) {
			printf("Error: ");
			switch (res) {
				case ESP_ERR_ULTRASONIC_PING:
					printf("Cannot ping (device is in invalid state)\n");
					break;
				case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
					printf("Ping timeout (no device found)\n");
					break;
				case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
					printf("Echo timeout (i.e. distance too big)\n");
					break;
				default:
					printf("%d\n", res);
			}
		} else {
			printf("Distance: %"PRIu32" cm, %.02f m\n", distance, distance / 100.0);
			cmdBuf.distance = distance;
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}


extern "C" void app_main()
{
        UltrasonicSensors teste;
        FallSensor teste3;
        PWM teste4;
        Movement direcao;
		Http App;

        direcao.run();

        int sim;

        xTaskCreate(ultrasonic, "ultrasonic", 1024*2, NULL, 2, NULL);

		    App.setup();
		
		while(1)
		{
			App.run();
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}


}
