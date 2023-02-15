#include <iostream>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h" 
#include "driver/i2c.h"
#include "driver/gpio.h"
// #include "driver/pulse_cnt.h"
#include "nvs_flash.h"
#include "esp_random.h"
#include "ObstaclesDetect.h"
#include "driver/touch_pad.h"

#include "Wheel.h"
#include "FallSensor.h"
#include "PWM.h"
#include "Movement.h"
#include "wifi.h"
#include "Http.h"
#include "http_client.h"

#define PIN_SDA 21
#define PIN_CLK 22
#define I2C_ADDRESS 0x68 // I2C address of MPU6050

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_PWR_MGMT_1   0x6B

/*
 * The following registers contain the primary data we are interested in
 * 0x3B MPU6050_ACCEL_XOUT_H
 * 0x3C MPU6050_ACCEL_XOUT_L
 * 0x3D MPU6050_ACCEL_YOUT_H
 * 0x3E MPU6050_ACCEL_YOUT_L
 * 0x3F MPU6050_ACCEL_ZOUT_H
 * 0x50 MPU6050_ACCEL_ZOUT_L
 * 0x41 MPU6050_TEMP_OUT_H
 * 0x42 MPU6050_TEMP_OUT_L
 * 0x43 MPU6050_GYRO_XOUT_H
 * 0x44 MPU6050_GYRO_XOUT_L
 * 0x45 MPU6050_GYRO_YOUT_H
 * 0x46 MPU6050_GYRO_YOUT_L
 * 0x47 MPU6050_GYRO_ZOUT_H
 * 0x48 MPU6050_GYRO_ZOUT_L
 */


#define STACK_SIZE 2048

std::string DEBUG = "DEBUG";
std::string nameMPU = "mpu6050";

std::string mpuTAG = "MEDIA";


int queroDormir = 1;
int machineState = 0;

TaskHandle_t xHandleINFRA = NULL;
TaskHandle_t xHandleMOV = NULL;
TaskHandle_t xHandleFSM = NULL;
int aceleracao = 5000;

QueueHandle_t filaDeInterrupcao;
Movement direcao;
ObstaclesDetect obstaculos;

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);

void task_mpu6050(void *ignore) {

    
	ESP_LOGI(nameMPU.c_str(), ">> mpu6050");
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = PIN_SDA;
	conf.scl_io_num = PIN_CLK;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 100000;
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

	i2c_cmd_handle_t cmd;
	vTaskDelay(200/portTICK_PERIOD_MS);

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
	i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1);
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
	i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
	i2c_master_write_byte(cmd, 0, 1);
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);


	uint8_t data[14];

	short accel_x;
	short accel_y;
	short accel_z;
    int counter = 0;
    int medium = 0;

	while(1) {
		// Tell the MPU6050 to position the internal register pointer to register
		// MPU6050_ACCEL_XOUT_H.
		cmd = i2c_cmd_link_create();
		ESP_ERROR_CHECK(i2c_master_start(cmd));
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_WRITE, 1));
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, 1));
		ESP_ERROR_CHECK(i2c_master_stop(cmd));
		ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
		i2c_cmd_link_delete(cmd);

		cmd = i2c_cmd_link_create();
		ESP_ERROR_CHECK(i2c_master_start(cmd));
		ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_ADDRESS << 1) | I2C_MASTER_READ, 1));

		ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data,   (i2c_ack_type_t)0));
		ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+1, (i2c_ack_type_t)0));
		ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+2, (i2c_ack_type_t)0));
		ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+3, (i2c_ack_type_t)0));
		ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+4, (i2c_ack_type_t)0));
		ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+5, (i2c_ack_type_t)1));

		//i2c_master_read(cmd, data, sizeof(data), 1);
		ESP_ERROR_CHECK(i2c_master_stop(cmd));
		ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_PERIOD_MS));
		i2c_cmd_link_delete(cmd);

		accel_x = (data[0] << 8) | data[1];
		accel_y = (data[2] << 8) | data[3];
		accel_z = (data[4] << 8) | data[5];
		// ESP_LOGI(nameMPU.c_str(), "accel_x: %d, accel_y: %d, accel_z: %d", accel_x, accel_y, accel_z);
        medium += accel_y;
        counter++;

        if (counter == 9) {
            counter = 0;
            medium /= 10;
            ESP_LOGI(mpuTAG.c_str(), "a MEDIA EH %d", medium);
			aceleracao = medium;
            medium = 0;
        }

		vTaskDelay(250/portTICK_PERIOD_MS);
	}
}

static void IRAM_ATTR gpio_isr_handler(void *args)
{
  int pino = (int)args;
  xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

void sensorQueda(void *params)
{
  FallSensor queda;
  gpio_num_t pino = (gpio_num_t)PIN_INFRA;

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
            vTaskDelay(750/ portTICK_PERIOD_MS);
        }

        // Habilitar novamente a interrupção
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
			
			ESP_LOGI(DEBUG.c_str(), "Primeiro modo: %d", direcao.getMode());

			vTaskDelay(2000 / portTICK_PERIOD_MS);

			if (valordoido >= 0) {
				if (valordoido == 3) {
					valordoido = 1;
				}
				temp = valordoido + 2;
			}

			direcao.setManualMode(temp);

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

bool isStopped(void)
{
	if ( aceleracao > -110 && aceleracao < 100 ) 
	{
		ESP_LOGW(mpuTAG.c_str(), "To parado!!!");
		return true;
	}
	return false;
}

void runWifi(void * pvParameters)
{
	Http App;
    App.setup();

	while(1) {
		App.run();
	
		while(App.Wifi.GetState() == WIFI::Wifi::state_e::CONNECTED){
			http_request();	
			vTaskDelay(10000 / portTICK_PERIOD_MS);	
			http_patch();	
			vTaskDelay(5000 / portTICK_PERIOD_MS);
		}
	}

}

void fsm(void * params)
{
	while(1){
		std::cout << "DEBUG : " << machineState << '\n';
		switch (machineState)
		{
		case 1:
			if ( queroDormir == 1 ) {
					direcao.run();
				if ( obstaculos.getBlockedMiddle() || (obstaculos.getBlockedLeft() && obstaculos.getBlockedRight()) || isStopped() ) {
					machineState=2;
				} 
				else 
				{ 
				if ( obstaculos.getBlockedLeft() ){
					machineState=3;
				}
				else
				{
				if (obstaculos.getBlockedRight()){
					machineState=4;
				}
				else
				{
					machineState=1;
				}
				}
				}

			} else direcao.stop();

			break;
		case 2:
			direcao.goDown();
			if ( obstaculos.getBlockedMiddle() || (obstaculos.getBlockedLeft() && obstaculos.getBlockedRight()) || isStopped() ) {
				if ( isStopped() ) aceleracao = 10000;

				machineState=2;
			} 
			else {
				machineState=1; 
			}	

			vTaskDelay(200 / portTICK_PERIOD_MS);
   
			break;
		case 3:
			direcao.goRight();

			if ( obstaculos.getBlockedLeft()) {
				if ( obstaculos.getBlockedRight() ) machineState=2;
				else machineState=3;
			} 
			else {
				machineState=1; 
			}	
			vTaskDelay(400 / portTICK_PERIOD_MS);
			break;
		case 4:
			direcao.goLeft();

			if ( obstaculos.getBlockedRight()) {
				if ( obstaculos.getBlockedLeft() ) machineState=2;
				else machineState=4;
			} 
			else {
				machineState=1; 
			}	

			vTaskDelay(400 / portTICK_PERIOD_MS);
			break;
		case 0: 
			vTaskDelay(5000 / portTICK_PERIOD_MS);
			machineState = 1;
			break;
		default:
			std::cout << "Error" << '\n';
			break;
		}
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}


}

extern "C" void app_main()
{

	short unsigned int touchValue = 1;

    ESP_ERROR_CHECK(touch_pad_init());
	touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
 	touch_pad_config((touch_pad_t)0, 0);

	gpio_reset_pin(GPIO_NUM_15);
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
    gpio_pulldown_dis(GPIO_NUM_15);
    gpio_pullup_dis(GPIO_NUM_15);

    gpio_set_level(GPIO_NUM_15, 1);

	gpio_reset_pin(GPIO_NUM_16);
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_INPUT);
    gpio_pulldown_dis(GPIO_NUM_16);
    gpio_pullup_en(GPIO_NUM_16);

	int ligada = 0;

	vTaskDelay(3000 / portTICK_PERIOD_MS);

	xTaskCreate(runWifi, "Wifi", 4096, NULL, 1, NULL);

	while ( !ligada )
	{
   		// touch_pad_read((touch_pad_t)0, &touchValue);
        // printf("toque =>> %d\n", touchValue);
	
				

		ligada = gpio_get_level(GPIO_NUM_16);
        printf("ligada =>> %d\n", ligada);	
	

		//ligada = gpio_get_level(GPIO_NUM_16);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}

	printf("INICIEI\n");

    gpio_set_level(GPIO_NUM_15, 0);

	PWM teste4;

	static uint8_t ucParameterToPass;

	Http App;

  	filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  	xTaskCreate(sensorQueda, "SensorInfra", STACK_SIZE, &ucParameterToPass, 1, &xHandleINFRA);


	gpio_install_isr_service(0);
  	gpio_isr_handler_add((gpio_num_t)PIN_INFRA, gpio_isr_handler, (void *) (gpio_num_t)PIN_INFRA);

	xTaskCreate(logicaMovimento, "Movimentacao", STACK_SIZE, &ucParameterToPass, 1 , &xHandleMOV);
	xTaskCreate(fsm, "fsm", STACK_SIZE, &ucParameterToPass, 1 , &xHandleFSM);


	vTaskDelay(10000 / portTICK_PERIOD_MS);

	xTaskCreate(task_mpu6050, "wtf", 2048, NULL, 1, NULL);


	App.setup();

	while(ligada)
	{
		App.run();
		ligada = gpio_get_level(GPIO_NUM_16);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}

    gpio_set_level(GPIO_NUM_15, 1);

	ESP_LOGI(DEBUG.c_str(), "quero dormir agora" );
	queroDormir = 0;

	vTaskDelay(2000 / portTICK_PERIOD_MS);

	vTaskDelete( xHandleFSM );
	vTaskDelete( xHandleMOV );
	vTaskDelete( xHandleINFRA );

	esp_restart();

}
