#ifndef __LUX_H__
#define __LUX_H__

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <sensor.h>

void lux_sensor_init();
void lux_sensor_polling_fn(struct k_timer *timer);
uint16_t getLuxValue();

#endif