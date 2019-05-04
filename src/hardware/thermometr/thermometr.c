#include <hardware/thermometr/thermometr.h>

#define TEMP_SENSOR_NAME    CONFIG_TEMP_NRF5_NAME

static struct device *thermometr_dev;

void thermometr_init()
{
    thermometr_dev = device_get_binding(TEMP_SENSOR_NAME);
	if (!thermometr_dev) {
		printk("> %s init failed\n\r", TEMP_SENSOR_NAME);
        return;
	}

    printk("THERMOMETR:> initialized\n\r");
}

s32_t get_temperature()
{
    int err;
    struct sensor_value temp_value;

    err = sensor_sample_fetch(thermometr_dev);
	if (err) {
		printk("THERMOMETR:> sensor_sample_fetch failed return: %d\n\r", err);
		return 0;
	}

	err = sensor_channel_get(thermometr_dev, SENSOR_CHAN_DIE_TEMP, &temp_value);
	if (err) {
		printk("THERMOMETR:> sensor_channel_get failed return: %d\n\r", err);
		return 0;
	}

	return sensor_value_to_double(&temp_value)*10;
}