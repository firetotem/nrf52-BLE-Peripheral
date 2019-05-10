#include <services/thermo_service.h>
#include <hardware/thermometr/thermometr.h>


#define THERMOMETR_TIME_DATA_UPDATE     1000

#define BT_UUID_HEALTH_THERMOMETR       BT_UUID_DECLARE_16(0x1809)
#define BT_UUID_TEMPERATURE_MEASUREMENT BT_UUID_DECLARE_16(0x2A1C)
#define BT_UUID_TEMPERATURE_TYPE        BT_UUID_DECLARE_16(0x2A1D)

/* User-defined variables */
static u8_t thermo_subscribed;
static temperature_measurement_t temperature_measurement = {0};
static struct bt_gatt_indicate_params indicate_param = {0};
extern struct bt_conn *default_connection;
static u8_t   temperature_type = 0x02;
/* Kernel's variables */
static struct bt_gatt_ccc_cfg thermo_ccc_cfg[BT_GATT_CCC_MAX] = {};

K_TIMER_DEFINE(thermo_ccc_indicate, thermo_measurement_indicate, NULL);


void thermo_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    thermo_subscribed = (value == BT_GATT_CCC_INDICATE) ? 1 : 0;
    
    thermo_subscribed ? printk("THERMO Service:> indicate enabled\n\r") : printk("THERMO Service:> indicate disabled\n\r");
    
    if (thermo_subscribed)
    {
        k_timer_start(&thermo_ccc_indicate, K_MSEC(0), K_MSEC(THERMOMETR_TIME_DATA_UPDATE));
    }
    else
    {
        k_timer_stop(&thermo_ccc_indicate);
    }    
}


BT_GATT_SERVICE_DEFINE(thermo_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_HEALTH_THERMOMETR),
    BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE_MEASUREMENT, BT_GATT_CHRC_INDICATE, BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(thermo_ccc_cfg, thermo_ccc_cfg_changed), 
    BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE_TYPE, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_temperature_type, NULL, &temperature_type),
);


void thermo_service_init()
{
    
}


static void thermo_measurement_indicate_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, u8_t err)
{
	if (err != 0)
	{
		printk("THERMO Service:> Indication failed: %d\n", err);
	}
}


void thermo_measurement_indicate(struct k_timer *timer)
{
    if (!thermo_subscribed)
    {
        return;
    }
 
    s32_t celcius = get_temperature();

    //printk("Device temp = %d\n\r", celcius);

    temperature_measurement.flags = 0x00;
    temperature_measurement.value[3] = 0xFF;
    temperature_measurement.value[2] = (celcius >> 16);
    temperature_measurement.value[1] = (celcius >> 8) & 0xFF;
    temperature_measurement.value[0] = celcius & 0xFF;
    

    indicate_param.attr = &thermo_svc.attrs[2];
    indicate_param.data = &temperature_measurement;
    indicate_param.len  = sizeof(temperature_measurement);
    indicate_param.func = thermo_measurement_indicate_cb;

    bt_gatt_indicate(default_connection, &indicate_param);
}

ssize_t	read_temperature_type(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset)
{
    printk("THERMO SERVICE:> read type\n\r");
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &temperature_type, sizeof(temperature_type));
}
