#ifndef __ADC_BAT_H__
#define __ADC_BAT_H__

#include <zephyr/types.h>
#include <zephyr.h>
#include <adc.h>

void adc_bat_init();
u8_t getBatteryLevel();

#endif