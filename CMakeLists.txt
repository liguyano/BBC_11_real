cmake_minimum_required(VERSION 3.25)
project(BBC_11_real C)

set(CMAKE_C_STANDARD 11)
include_directories(.)
INCLUDE_DIRECTORIES(E:\\Keil_v5\\C51\\INC\\Atmel E:\\Keil_v5\\C51\\INC\\STC E:\\Keil_v5\\C51\\INC)
add_executable(BBC_11_real main.c DS1302.c DS1302.h DS1302.h DS18B02.c DS18B02.h MAtrix_key.c PCF8591.c PCF8591.h PCF8591.h)
