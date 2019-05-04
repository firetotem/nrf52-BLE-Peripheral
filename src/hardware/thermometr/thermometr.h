#ifndef __THERMOMETR_H__
#define __THERMOMETR_H__

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <sensor.h>


void thermometr_init();
s32_t get_temperature();


#endif