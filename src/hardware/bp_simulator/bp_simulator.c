#include <hardware/bp_simulator/bp_simulator.h>

struct bp_measure
{
    uint16_t systolic;
    uint16_t diastolyc;
    uint16_t pulse_rate;
};

static struct bp_measure measure;

void bp_simulate()
{
    uint32_t rand = sys_rand32_get();
    static uint8_t i;

    if (!(i%2))
    {
        measure.systolic = 120 + rand%20;
        measure.diastolyc = 70 + rand%10;
        measure.pulse_rate = 75 + rand%15;
    }
}

uint16_t get_systolic()
{
    return measure.systolic;
}

uint16_t get_diastolyc()
{
    return measure.diastolyc;
}

uint16_t get_mean_arterial()
{
    return (measure.systolic + measure.diastolyc)/2;
}

uint16_t get_pulse_rate()
{
    return measure.pulse_rate;
}