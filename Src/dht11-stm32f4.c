#include "dht11-stm32f4.h"

GPIO_TypeDef *DHT11_Port;
uint16_t DHT11_Pin;
TIM_HandleTypeDef *_htim;

/**
 * @brief Sets the specified pin to output mode.
 * 
 * This function configures the microcontroller pin to act as an output,
 * allowing it to drive signals to connected devices or components.
 * 
 * @note Ensure that the pin is correctly initialized before calling this function.
 */
static void set_pin_output();



/**
 * @brief Configures the GPIO pin as an input.
 *
 * This function sets the specified GPIO pin to input mode, allowing it to read
 * data from external devices or sensors. It is typically used in scenarios
 * where the pin needs to receive signals rather than send them.
 */
static void set_pin_input();



/**
 * @brief Delays the program execution for a specified number of microseconds.
 * 
 * This function provides a delay in microseconds.
 * 
 * @param us The number of microseconds to delay.
 */
static void delay_us(uint16_t us);



/**
 * @brief Sends the start signal to the DHT11 sensor.
 * 
 * This function initiates communication with the DHT11 sensor by sending a start signal.
 * The sensor will respond with a signal indicating it is ready to transmit data.
 * 
 * @return DHT11_Status 
 */
static DHT11_Status start_signal();



/**
 * @brief Reads a byte of data from the DHT11 sensor.
 * 
 * This function reads a single byte of data from the DHT11 sensor and stores it
 * in the provided byte pointer. It returns the status of the read operation.
 * 
 * @param[out] byte Pointer to a uint8_t variable where the read byte will be stored.
 * @return DHT11_Status Status of the read operation.
 */
static DHT11_Status read_byte(uint8_t *byte);



/**
 * @brief  Waits until the specified GPIO pin reaches the desired state or the maximum time is exceeded.
 * @param  set_reset: The desired state of the GPIO pin (0 or 1).
 * @param  max_time: The maximum time to wait in timer ticks.
 * @retval DHT11_Status: Returns DHT11_OK if the desired state is reached within the maximum time,
 *                       otherwise returns DHT11_TIMEOUT.
 */
static DHT11_Status wait_until(uint8_t set_reset, int max_time);




DHT11_Status DHT11_init(
  GPIO_TypeDef *GPIO_Port,
  uint16_t GPIO_Pin,
  TIM_HandleTypeDef *htim
)
{
  DHT11_Port = GPIO_Port;
  DHT11_Pin = GPIO_Pin;
  _htim = htim;
  HAL_TIM_Base_Start(htim);
  set_pin_output();
}




DHT11_Status DHT11_read(uint16_t *temp, uint16_t *rhm)
{
  uint8_t data[5] ={0};
  DHT11_Status status = start_signal();
  for (uint8_t i = 0; i < 5; i++)
  {
    status = read_byte(&data[i]);
  }
  *rhm = (uint16_t)data[0];
  *temp = (uint16_t)data[2];

  return status;
}



static DHT11_Status start_signal()
{
  DHT11_Status status;
  set_pin_output();
  HAL_GPIO_WritePin(DHT11_Port,DHT11_Pin, GPIO_PIN_RESET);
  HAL_Delay(18);
  HAL_GPIO_WritePin(DHT11_Port, DHT11_Pin, GPIO_PIN_SET);
  status = wait_until(GPIO_PIN_RESET, 50);
  set_pin_input();
  delay_us(140);
  return status;
}



static DHT11_Status read_byte(uint8_t *byte)
{
  *byte = 0;
  DHT11_Status status = DHT11_ERR;
  for (uint8_t i = 0; i < 8; i++)
  {
    status = wait_until(GPIO_PIN_SET, 60);

    delay_us(30);
    
    if (HAL_GPIO_ReadPin(DHT11_Port, DHT11_Pin) == GPIO_PIN_SET)
    {
      *byte |= (1 << (7 - i));
       status = DHT11_OK;
    }
    status = wait_until(GPIO_PIN_RESET, 70);
  }
  return status;
}



static void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(_htim,0);
	while (__HAL_TIM_GET_COUNTER(_htim) < us);
}



static DHT11_Status wait_until (uint8_t set_reset, int max_time)
{ 
  int start_time = __HAL_TIM_GET_COUNTER(_htim);
  int delta_time = 0;

  while (HAL_GPIO_ReadPin(DHT11_Port, DHT11_Pin) != set_reset && (delta_time <= max_time))
  {
    delta_time = __HAL_TIM_GET_COUNTER(_htim) - start_time;
  }

  if (delta_time >= max_time)
  {
    return DHT11_TIMEOUT;
  }
  
  return DHT11_OK;
}



static void set_pin_output()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_Port, &GPIO_InitStruct);
}



static void set_pin_input()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_Port, &GPIO_InitStruct);
}