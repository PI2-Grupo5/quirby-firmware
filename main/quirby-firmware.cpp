#include <iostream>

#include "IOPin.h"
#include "IRPin.h"
#include "WheelPin.h"
#include "UltrasonicSensors.h"

extern "C" void app_main()
{
		std::cout << int(IOPin::PIN_LED) << '\n';
		std::cout << int(IRPin::PIN_INFRA) << '\n';
		std::cout << int(WheelPin::PIN_IN1) << '\n';
        UltrasonicSensors teste;
        std::cout << teste.x << '\n';
}
