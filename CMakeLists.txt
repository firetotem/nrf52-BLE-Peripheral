# Minimal requred version CMake
cmake_minimum_required(VERSION 3.13.1)

# Defines
set(BOARD nrf52_adafruit_feather)
set(KCONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig)


include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(bt_peripheral)


#FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE src/main.c
               src/services/lux_service.c
               src/services/batery_service.c
               src/services/thermo_service.c
               src/services/immediate_alert.c
               src/hardware/led/led.c
               src/hardware/lux_sensor/lux.c
               src/hardware/adc/adc_bat.c
               src/hardware/thermometr/thermometr.c)


zephyr_library_include_directories(src)
zephyr_library_include_directories($ENV{ZEPHYR_BASE}/samples/bluetooth)