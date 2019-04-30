# Minimal requred version CMake
cmake_minimum_required(VERSION 3.13.1)

# Defines
set(BOARD nrf52_adafruit_feather)
set(KCONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig)


include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(bt_peripheral)


#FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE src/main.c
               $ENV{ZEPHYR_BASE}/samples/bluetooth/gatt/bas.c)


zephyr_library_include_directories($ENV{ZEPHYR_BASE}/samples/bluetooth)