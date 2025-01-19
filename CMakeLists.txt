cmake_minimum_required(VERSION 3.22)
add_library(dht11-stm32f4 STATIC
    Src/dht11-stm32f4.c
)

target_include_directories(dht11-stm32f4 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(dht11-stm32f4 PRIVATE stm32cubemx)