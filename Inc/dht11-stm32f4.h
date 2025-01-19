#ifndef DHT11_STM32F4_H
#define DHT11_STM32F4_H

#include "stm32f4xx_hal.h"

#define TIMEOUT 100

typedef enum {
    DHT11_OK = 0,
    DHT11_CHECKSUM_ERR = -1,
    DHT11_TIMEOUT = -2,
    DHT11_ERR = -3
} DHT11_Status;


/**
 * @brief Initializes the Timer and GPIO.    
 * This function handles the GPIO initialization, so no prior configuration via CubeMX is required.
 * 
 * @param GPIO_Port: GPIO port to be initialized.
 * @param GPIO_Pin: GPIO pin to be initialized.
 * @param htim: Timer handle to be used. Ensure that the timer's prescaler is set correctly, 
 *              so that one timer tick equals one microsecond. 
 *              For the STM32F401RE with default CubeMX settings, the prescaler should be set to 84.
 */
DHT11_Status DHT11_init(
  GPIO_TypeDef *GPIO_Port,
  uint16_t GPIO_Pin,
  TIM_HandleTypeDef *htim
);



/**
 * @brief Reads the temperature and relative humidity from the DHT11 sensor.
 * 
 * @param[out] temp Pointer to a variable where the temperature value will be stored.
 *                  The temperature is represented as an integer value.
 * @param[out] rhm  Pointer to a variable where the relative humidity value will be stored.
 *                  The relative humidity is represented as an integer value.
 * 
 * @return DHT11_Status Status of the read operation. It indicates whether the read was successful or if there was an error.
 */
DHT11_Status DHT11_read(uint16_t *temp, uint16_t *rhm);




#endif