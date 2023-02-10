#pragma once

#include <iostream>
#include <esp_log.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "UltrasonicPin.h"
#include "ultrasonic.h"

extern TaskHandle_t xHandleSOML;
extern TaskHandle_t xHandleSOMM;
extern TaskHandle_t xHandleSOMR;

extern uint32_t distanceL;
extern uint32_t distanceM;
extern uint32_t distanceR;

extern uint8_t ucDParameterToPass;

#define STACK_SIZE 2048

#define MAX_DISTANCE_CM 400 // 5m max

#define CMD_START 100
#define CMD_STOP 200
#define CMD_MEASURE 300
#define CMD_CLEAR 400

typedef struct {
    uint16_t command;
    uint32_t distance;
    TaskHandle_t taskHandle;
} CMD_t;

void ultrasonic(void *pvParamters);
class UltrasonicSensors {
public:
    UltrasonicSensors();

    void setupPin(gpio_num_t pinSet);

    int getTrip() const;
    void setTrip(int trip);

    int getEcho() const;
    void setEcho(int echo);

private:
    int trip;
    int echo;

};