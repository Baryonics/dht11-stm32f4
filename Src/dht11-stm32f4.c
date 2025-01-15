#include "dht11-stm32f4.h"

/* Global Variables */
static GPIO_TypeDef *DHT11_Port;
static uint16_t DHT11_Pin;
static TIM_HandleTypeDef _htim;

/* Function dummys */
void delay_us(uint16_t us);
void set_pin_input();
void set_pin_output();
void delay_us(uint16_t us);
void start_signal();
uint8_t read_byte();
int read_data(uint8_t data[5]);
DHT11_Status wait_until(uint8_t signal);


void dht11_init(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin,TIM_HandleTypeDef htim)
{
  DHT11_Port = GPIO_Port;
  DHT11_Pin = GPIO_Pin;
  _htim = htim;
  HAL_TIM_Base_Start(&htim);

  set_pin_output();
  HAL_GPIO_WritePin(DHT11_Port, DHT11_Pin, GPIO_PIN_SET);
}


unsigned int get_T()
{
  uint8_t data[5];
  int result = read_data(data);
  
  if (result != DHT11_OK)
  {
    return result;
  }
  
  unsigned int temperature = data[2];
  
  return temperature;
}


int read_data(uint8_t data[5])
{  
  start_signal();

  for (int i = 0; i < 5; i++)
  {
    if (HAL_GPIO_ReadPin(DHT11_Port, DHT11_Pin) == GPIO_PIN_RESET)
    {
      data[i] = read_byte();
    }
    else
    {
      return DHT11_ERR;
    }
  }

  if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
  {
    return DHT11_CHECKSUM_ERR;
  }
  
  return DHT11_OK;
}


/**
 * @brief Initiates communication with the DHT11 sensor and sets the specified Pin to input
 */
void start_signal(void)
{
  HAL_GPIO_WritePin(DHT11_Port,DHT11_Pin, GPIO_PIN_RESET);
  HAL_Delay(20);
  HAL_GPIO_WritePin(DHT11_Port, DHT11_Pin, GPIO_PIN_SET);
  delay_us(40);
  set_pin_input();
  
  wait_until(GPIO_PIN_RESET);
  
}


/**
 * @brief Reads one byte transmitted by the DHT11 sensor
 */
uint8_t read_byte()
{
  uint8_t byte = 0;
  DHT11_Status status;

  for (uint8_t i = 0; i < 8; i++)
  {

    status = wait_until(GPIO_PIN_RESET);
    
    delay_us(35);

    if (HAL_GPIO_ReadPin(DHT11_Port,DHT11_Pin) == GPIO_PIN_SET)
    {
      byte |= (1 << (7 - i));
    }

    status = wait_until(GPIO_PIN_SET);
  }
  return byte;
}


DHT11_Status wait_until(uint8_t signal)
{
  if (signal != GPIO_PIN_SET || signal != GPIO_PIN_RESET)
  {
    return DHT11_ERR;
  }
  

  __HAL_TIM_SET_COUNTER(&_htim, 0);
  while (HAL_GPIO_ReadPin(DHT11_Port, DHT11_Pin) != signal)
  {
    if (__HAL_TIM_GET_COUNTER(&_htim) >= TIMEOUT)
    {
      return DHT11_TIMEOUT;
    }
  }
  return DHT11_OK;
}


/**
 * @brief Delays for a given time in microseconds.
 *        Note that the specified timer needs to be initialized correctly
 */
void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&_htim,0);
	while (__HAL_TIM_GET_COUNTER(&_htim) < us);
}


/**
 * @brief Sets the specified pin to output mode
 */
void set_pin_output()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_Port, &GPIO_InitStruct);
}


/**
 * @brief Sets the specified pin to input mode
 */
void set_pin_input()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_Port, &GPIO_InitStruct);
}