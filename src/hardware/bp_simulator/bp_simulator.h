#ifndef __BP_SIMULATOR_H__
#define __BP_SIMULATOR_H__

#include <zephyr/types.h>
#include <zephyr.h>

void bp_simulate();
uint16_t get_systolic();
uint16_t get_diastolyc();
uint16_t get_mean_arterial();
uint16_t get_pulse_rate();

#endif