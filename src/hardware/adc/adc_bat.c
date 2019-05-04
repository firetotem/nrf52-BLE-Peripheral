
#include <hardware/adc/adc_bat.h>

#define ADC_NAME        "ADC_0"
#define BAT_CHANNEL     8
#define ADC_RESOLUTION  12
#define BAT_MAX_VALUE   (((1<<ADC_RESOLUTION)/3)*2.63)
#define BAT_MIN_VALUE   (((1<<ADC_RESOLUTION)/3)*2)


static struct device *adc_device;
static struct adc_channel_cfg adc_cfg = {0};
static struct adc_sequence    adc_seq = {0};

static s16_t adc_sample = 0;

void adc_bat_init()
{
    adc_device = device_get_binding(ADC_NAME);
    if (adc_device == NULL)
    {
        printk("> %s init failed\n\r", ADC_NAME);
        return;
    }

    adc_cfg.gain = ADC_GAIN_1_5;
	adc_cfg.reference = ADC_REF_INTERNAL;
    adc_cfg.acquisition_time = ADC_ACQ_TIME_DEFAULT;
    adc_cfg.channel_id = BAT_CHANNEL;
    adc_cfg.input_positive = BAT_CHANNEL;

    if (adc_channel_setup(adc_device, &adc_cfg))
    {
        printk("> %s init failed\n\r", ADC_NAME);
        return;
    } 

    printk("%s:> inititialized\n\r", ADC_NAME);
}

u8_t getBatteryLevel()
{
    adc_seq.options = NULL;
    adc_seq.channels = BIT(BAT_CHANNEL);
	adc_seq.buffer = &adc_sample;	
	adc_seq.buffer_size = sizeof(adc_sample);
    adc_seq.resolution = ADC_RESOLUTION;
	
    if (adc_read(adc_device, &adc_seq))
    {
        printk("> %s getting sample failed\n\r", ADC_NAME);
    }

    //printk("> %s sample = %d\n\r", ADC_NAME, adc_sample);

    return ((adc_sample - BAT_MIN_VALUE)*100U/BAT_MAX_VALUE);
}