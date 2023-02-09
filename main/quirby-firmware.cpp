#include <iostream>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h" 
// #include "driver/pulse_cnt.h"
#include "nvs_flash.h"
#include "esp_random.h"

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

#define STACK_SIZE 2048

std::string DEBUG = "DEBUG";

int queroDormir = 1;

TaskHandle_t xHandleINFRA = NULL;
TaskHandle_t xHandleMOV = NULL;

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


	QueueHandle_t filaDeInterrupcao;
	Movement direcao;

static void IRAM_ATTR gpio_isr_handler(void *args)
{
  int pino = (int)args;
  xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

void trataInterrupcaoBotao(void *params)
{
  FallSensor queda;
  gpio_num_t pino = (gpio_num_t)PIN_INFRA;
  int contador = 0;

  while(true)
  {
    if(xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY))
    {
      // De-bouncing
      int estado = gpio_get_level(pino);
      if(estado == 1)
      {
        gpio_isr_handler_remove((gpio_num_t)PIN_INFRA);
        while(gpio_get_level(pino) == estado)
        {
			direcao.setManualMode(3);
			direcao.run();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        // Habilitar novamente a interrupção
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_isr_handler_add(pino, gpio_isr_handler, (void *) pino);
      }

    }
  }
}

void logicaMovimento( void * pvParameters)
{
	int valordoido = 0;
	int temp = 0;

	while(1)
	{
		if(queroDormir)
		{
			valordoido = (int)esp_random();
			valordoido = valordoido % 3;

			direcao.setManualMode(1);
			direcao.run();
			
			ESP_LOGI(DEBUG.c_str(), "Primeiro modo: %d", direcao.getMode());

			vTaskDelay(2000 / portTICK_PERIOD_MS);

			if (valordoido >= 0) {
				temp = valordoido + 2;
			}

			direcao.setManualMode(temp);
			direcao.run();

			ESP_LOGI(DEBUG.c_str(), "Primeiro modo: %d", direcao.getMode());

			vTaskDelay(2000 / portTICK_PERIOD_MS);
		} else 
		{
			direcao.stop();
			vTaskSuspend(xHandleMOV);
		}
	}

	ESP_LOGI(DEBUG.c_str(), "Desligando motores");

	direcao.stop();
	

}

extern "C" void app_main()
{
	UltrasonicSensors teste;
	PWM teste4;


	static uint8_t ucParameterToPass;


	Http App;

  	filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  	xTaskCreate(trataInterrupcaoBotao, "SensorInfra", STACK_SIZE, &ucParameterToPass, 1, &xHandleINFRA);


	gpio_install_isr_service(0);
  	gpio_isr_handler_add((gpio_num_t)PIN_INFRA, gpio_isr_handler, (void *) (gpio_num_t)PIN_INFRA);

	xTaskCreate(logicaMovimento, "Movimentacao", STACK_SIZE, &ucParameterToPass, 1 , &xHandleMOV);

	
	vTaskDelay(20000 / portTICK_PERIOD_MS);

	ESP_LOGI(DEBUG.c_str(), "quero dormir agora" );
	queroDormir = 0;

	vTaskDelay(60000 / portTICK_PERIOD_MS);

	vTaskDelete( xHandleMOV );
	vTaskDelete( xHandleINFRA );


}
