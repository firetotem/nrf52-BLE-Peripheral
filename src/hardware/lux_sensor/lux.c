#include <hardware/lux_sensor/lux.h>

#define LUX_SENSOR_NAME         CONFIG_TSL2561_NAME
#define LUX_SENSOR_POLLING_TIME 1000

static struct device *sensor_dev;
static struct sensor_value lux = {0};
static bool   m_initialized = false;

K_TIMER_DEFINE(lux_timer, lux_sensor_polling_fn, NULL);

void lux_sensor_init()
{
    if (m_initialized)
    {
        return;
    }
    
    sensor_dev = device_get_binding(LUX_SENSOR_NAME);
    
    if (sensor_dev == NULL)
    {
        printk("Lux sensor init failed\n\r");
        return;
    }
    printk("Lux sensor initialized\n\r");
    
    m_initialized = true;
    //k_timer_start(&lux_timer, K_MSEC(1000), K_MSEC(LUX_SENSOR_POLLING_TIME));
}

void lux_sensor_polling_fn(struct k_timer *timer)
{
    if (m_initialized)
    {
        if (sensor_channel_get(sensor_dev, SENSOR_CHAN_LIGHT, &lux) < 0)
        {
            printk("Cannot read data from %s \n\r", LUX_SENSOR_NAME);     
        }
    }
}

uint16_t getLuxValue()
{
    if (sensor_channel_get(sensor_dev, SENSOR_CHAN_LIGHT, &lux) < 0)
    {
        printk("Cannot read data from %s \n\r", LUX_SENSOR_NAME);     
    }
    return (uint16_t)lux.val1;
}
