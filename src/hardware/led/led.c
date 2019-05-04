
#include <hardware/led/led.h>

#define LED_GPIO        LED0_GPIO_CONTROLLER
#define LED_RED         LED0_GPIO_PIN
#define LED_BLUE        LED1_GPIO_PIN

struct device *gpio_dev;
static volatile bool    flash_led[2]    = {false, false};
static volatile bool    led_states[2]   = {false, false};
static volatile u32_t   led_pin[2]      = {LED_RED, LED_BLUE};
static volatile bool    flash_on        = false;
static volatile bool    flashing        = false;
static volatile u8_t    flash_control_count = 8;

static K_TIMER_DEFINE(state_timer, state_callback, NULL);
static K_TIMER_DEFINE(flash_timer, flash_callback, NULL);

void led_init()
{
    int err;
    
    gpio_dev = device_get_binding(LED_GPIO);
    
    if (gpio_dev == NULL)
    {
        printk("led:> can not get gpio device\n\r");
        return;
    }
    
    err = gpio_pin_configure(gpio_dev, LED_RED, GPIO_DIR_OUT);
    if (err)
    {
        printk("Red led init failed (err = %d)\n\r", err);
        return;
    }
        
    err = gpio_pin_configure(gpio_dev, LED_BLUE, GPIO_DIR_OUT);
    if (err)
    {
        printk("Blue led init failed (err = %d)\n\r", err);
        return;
    }
    printk("LED:> initialized\n\r");
}

void state_callback(struct k_timer *timer)
{
    for(int i = 0; i < 2; i++)
    {
        if (led_states[i])
        {
            gpio_pin_write(gpio_dev, led_pin[i], 1);
            led_states[i] = true;
        }
        else
        {
            gpio_pin_write(gpio_dev, led_pin[i], 0);
            led_states[i] = false;
        }
        
    }
}

void flash_callback(struct k_timer *timer)
{
    for (int i = 0; i < 2; i++)
	{
		if (flash_led[i] == true)
		{
			if (flash_on)
			{
				// // printk("LED %d on\r\n", i);
				gpio_pin_write(gpio_dev, led_pin[i], 1);
				led_states[i] = true;
			}
			else
			{
				// // printk("LED %d off\r\n", i);
				gpio_pin_write(gpio_dev, led_pin[i], 0);
				led_states[i] = false;
			}
		}
	}
	flash_on = !flash_on;
	flash_control_count--;
	if (flash_control_count == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			flash_led[i] = false;
			led_states[i] = false;
		}
		flashing = false;
		// // printk("stopping flashing\n");
		k_timer_stop(&flash_timer);
	}
}

void led_set_state(u8_t led, bool state)
{
    led_states[led] = state;
    k_timer_start(&state_timer, K_MSEC(0), 0);
}

void led_flash_state(bool led_red, bool led_blue, u8_t times)
{
    if (flashing)
    {
        return;
    }    
    flash_led[0] = led_red;
    flash_led[1] = led_blue;
    flash_control_count = times * 2;
    flashing = true;
    flash_on = true;
    k_timer_start(&flash_timer, K_MSEC(0), K_MSEC(LED_SLEEP_TIME));
}