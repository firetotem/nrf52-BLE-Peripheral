#ifndef __LED_H__
#define __LED_H__

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <gpio.h>


#define LED_SLEEP_TIME  500U
#define LED_RED_PIN     0
#define LED_BLUE_PIN    1

void led_init();
void led_set_state(u8_t led, bool state);
void state_callback(struct k_timer *timer);
void flash_callback(struct k_timer *timer);
void led_flash_state(bool led_red, bool led_blue, u8_t times);

#endif