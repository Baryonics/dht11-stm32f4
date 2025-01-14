#ifndef DHT11_STM32F4_H
#define DHT11_STM32F4_H

#include "stm32f4xx_hal.h"

typedef enum {
    DHT11_OK = 0,
    DHT11_ERROR = 1,
    DHT11_TIMEOUT = 2
} DHT11_Status;

void dht11_init(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin,TIM_HandleTypeDef htim);

void start_signal();

uint8_t read_byte();


#endif // DHT11_STM32F4_H