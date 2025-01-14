cmake_minimum_required(VERSION 3.22)

# Library erstellen
add_library(dht11-stm32f4 STATIC
    Src/dht11-stm32f4.c
)

# Include-Verzeichnis hinzufügen
target_include_directories(dht11-stm32f4 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

# STM32 HAL als Abhängigkeit hinzufügen
target_link_libraries(dht11-stm32f4 PRIVATE stm32cubemx)