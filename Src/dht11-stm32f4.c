#include "dht11-stm32f4.h"

GPIO_TypeDef *_GPIO_Port;
uint16_t _GPIO_Pin;
TIM_HandleTypeDef _htim;

void dht11_init(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin,TIM_HandleTypeDef htim)
{
  _GPIO_Port = GPIO_Port;
  _GPIO_Pin = GPIO_Pin;
  _htim = htim;
  HAL_TIM_Base_Start(&htim);
}

void set_pin_output();
void set_pin_input();
void delay_us(uint16_t);

void start_signal(void)
{
  set_pin_output();
  HAL_GPIO_WritePin(_GPIO_Port, _GPIO_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(_GPIO_Port,_GPIO_Pin, GPIO_PIN_RESET);
  HAL_Delay(18);
  HAL_GPIO_WritePin(_GPIO_Port, _GPIO_Pin, GPIO_PIN_SET);
  delay_us(35);
  set_pin_input();
}

uint8_t read_byte()
{
  uint8_t byte = 0;

  for (uint8_t i = 0; i < 8; i++)
  {
    while (HAL_GPIO_ReadPin(_GPIO_Port, _GPIO_Pin) == GPIO_PIN_RESET);
    
    while (HAL_GPIO_ReadPin(_GPIO_Port, _GPIO_Pin) == GPIO_PIN_SET);
    delay_us(50);

    if (HAL_GPIO_ReadPin(_GPIO_Port,_GPIO_Pin) == GPIO_PIN_SET)
    {
      byte |= (1 << (7 - i));
    }

    while (HAL_GPIO_ReadPin(_GPIO_Port, _GPIO_Pin) == GPIO_PIN_SET);
    
  }
  return byte;
}

void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&_htim,0);
	while (__HAL_TIM_GET_COUNTER(&_htim) < us);
}


void set_pin_output()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = _GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(_GPIO_Port, &GPIO_InitStruct);
}


void set_pin_input()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = _GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(_GPIO_Port, &GPIO_InitStruct);
}