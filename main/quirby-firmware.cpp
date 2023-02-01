#include <iostream>

#include <IOPin.h>
#include <IRPin.h>
#include <WheelPin.h>

extern "C" void app_main()
{
		std::cout << int(IOPin::PIN_LED) << '\n';
		std::cout << int(IRPin::PIN_INFRA) << '\n';
		std::cout << int(WheelPin::PIN_IN1) << '\n';

	std::cout << "Hello ESP32" << '\n';
}
