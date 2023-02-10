#include "UltrasonicSensors.h"

TaskHandle_t xHandleSOML = NULL;
TaskHandle_t xHandleSOMM = NULL;
TaskHandle_t xHandleSOMR = NULL;

uint32_t distanceL = 100;
uint32_t distanceM = 100;
uint32_t distanceR = 100;
uint8_t ucDParameterToPass = NULL;

void ultrasonicL(void *pvParamters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
    std::string ERRLOGIL = "ULTRA L";
	CMD_t cmdBufL;
	cmdBufL.command = CMD_MEASURE;
	cmdBufL.taskHandle = xTaskGetCurrentTaskHandle();

	ultrasonic_sensor_t sensor = {
		.trigger_pin = (gpio_num_t)PIN_TRIP_L,
		.echo_pin = (gpio_num_t)PIN_ECHO_L
	};

	ultrasonic_init(&sensor);

	while (true) {

		esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distanceL);
		if (res != ESP_OK) {
	            ESP_LOGI(ERRLOGIL.c_str(), "Erro esquerda");
			
		} else {
			//printf("Distance: %"PRIu32" cm, %.02f m\n", distance, distance / 100.0);
			cmdBufL.distance = distanceL;
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void ultrasonicM(void *pvParamters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
    std::string ERRLOGIM = "ULTRA M";
	CMD_t cmdBufM;
	cmdBufM.command = CMD_MEASURE;
	cmdBufM.taskHandle = xTaskGetCurrentTaskHandle();

	ultrasonic_sensor_t sensor = {
		.trigger_pin = (gpio_num_t)PIN_TRIP_M,
		.echo_pin = (gpio_num_t)PIN_ECHO_M
	};

	ultrasonic_init(&sensor);

	while (true) {

		esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distanceM);
		if (res != ESP_OK) {
	            ESP_LOGI(ERRLOGIM.c_str(), "Erro meio");
			
		} else {
			//printf("Distance: %"PRIu32" cm, %.02f m\n", distance, distance / 100.0);
			cmdBufM.distance = distanceM;
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void ultrasonicR(void *pvParamters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
    std::string ERRLOGIR = "ULTRA R";
	CMD_t cmdBufR;
	cmdBufR.command = CMD_MEASURE;
	cmdBufR.taskHandle = xTaskGetCurrentTaskHandle();

	ultrasonic_sensor_t sensor = {
		.trigger_pin = (gpio_num_t)PIN_TRIP_R,
		.echo_pin = (gpio_num_t)PIN_ECHO_R
	};

	ultrasonic_init(&sensor);

	while (true) {

		esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distanceR);
		if (res != ESP_OK) {
	            ESP_LOGI(ERRLOGIR.c_str(), "Erro direita");
			
		} else {
			//printf("Distance: %"PRIu32" cm, %.02f m\n", distance, distance / 100.0);
			cmdBufR.distance = distanceR;
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

UltrasonicSensors::UltrasonicSensors() {
    // setupPin((gpio_num_t)PIN_ECHO_R);
    // setupPin((gpio_num_t)PIN_TRIP_R);
    // setupPin((gpio_num_t)PIN_ECHO_M);
    // setupPin((gpio_num_t)PIN_TRIP_M);
    // setupPin((gpio_num_t)PIN_ECHO_L);
    // setupPin((gpio_num_t)PIN_TRIP_L);
    xTaskCreate(ultrasonicL, "UltraSomLeft", STACK_SIZE, &ucDParameterToPass, 1 , &xHandleSOML);
    xTaskCreate(ultrasonicM, "UltraSomMiddle", STACK_SIZE, &ucDParameterToPass, 1 , &xHandleSOMM);
    xTaskCreate(ultrasonicR, "UltraSomRight", STACK_SIZE, &ucDParameterToPass, 1 , &xHandleSOMR);

}

void UltrasonicSensors::setupPin(gpio_num_t pinSet) {
    gpio_reset_pin(pinSet);
    gpio_set_direction(pinSet, GPIO_MODE_INPUT);
    gpio_pulldown_en(pinSet);
    gpio_pullup_dis(pinSet);

}

int UltrasonicSensors::getTrip() const {
    return trip;
}

void UltrasonicSensors::setTrip(int trip) {
    UltrasonicSensors::trip = trip;
}

int UltrasonicSensors::getEcho() const {
    return echo;
}

void UltrasonicSensors::setEcho(int echo) {
    UltrasonicSensors::echo = echo;
}

