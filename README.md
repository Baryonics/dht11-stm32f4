# DHT11 STM32F4 Library

This library provides an interface for the DHT11 temperature and humidity sensor using STM32F4 microcontrollers, specifically tested with the STM32 Nucleo F401RE.

## Features

- Read temperature and humidity data from the DHT11 sensor
- Simple API for easy integration
- Non-blocking data acquisition

## Requirements

- STM32F4 microcontroller (tested with STM32 Nucleo F401RE)
- STM32CubeMX and HAL libraries
- DHT11 sensor

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/Baryonics/dht11-stm32f4.git
    ```
2. Add the entire `dht11-stm32f4` folder to your project.
3. Include the DHT11 library in your `CMakeLists.txt` file:
    ```cmake
    add_subdirectory(dht11-stm32f4)
    target_link_libraries(your_project_name PRIVATE dht11-stm32f4)
    ```
4. Include the DHT11 library in your main code:
    ```c
    #include "dht11-stm32f4.h"
    ```

## Usage

1. Initialize the DHT11 sensor:
    ```c
    DHT11_Init(DHT11_DATA_Port, DHT11_DATA_Pin, htim);
    ```
2. Read data from the sensor:
    ```c
    uint16_t temp;
    uint16_t rh;

    DHT11_Status dht11_status = dht11_read(&temp, &rh);
    ```

## API Reference

### `void DHT11_Init(void)`

Initializes the DHT11 sensor.

### `DHT11_Status DHT11_Read(uint16_t *temp, uint16_t *rh)`

Reads temperature and humidity data from the DHT11 sensor.

- `*temp`: Pointer to a variable to store the read temperature data.
- `*rh`: Pointer to a variable to store the read humidity data.
- Returns `DHT11_OK` on success, or an error code on failure.
