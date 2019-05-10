
#include <services/batery_service.h>
#include <hardware/adc/adc_bat.h>

static u8_t battery_level = 100;
static u8_t battery_level_subscribed;
static struct bt_gatt_ccc_cfg bas_ccc_cfg[BT_GATT_CCC_MAX] = {};
static struct bt_gatt_cpf bas_cpf;
extern struct bt_conn *default_connection;

K_TIMER_DEFINE(notify_timer, batery_notify, NULL);


ssize_t	bas_read_fn(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset)
{
    
    battery_level = getBatteryLevel();
    
    printk("BAS Service:> read request\n\r");

    return bt_gatt_attr_read(conn, attr, buf, len, offset, &battery_level, sizeof(battery_level));
}


void bas_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    battery_level_subscribed = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;

    battery_level_subscribed ? printk("BAS Service:> notyfy enabled\n\r") : printk("BAS Service:> notyfy disabled\n\r");

    if (battery_level_subscribed)
    {
        k_timer_start(&notify_timer, K_MSEC(0), K_MSEC(1000));
    }
    else
    {
        k_timer_stop(&notify_timer);
    }    
}


BT_GATT_SERVICE_DEFINE(batery_level_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_BAS),
    BT_GATT_CHARACTERISTIC(BT_UUID_BAS_BATTERY_LEVEL, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, bas_read_fn, NULL, &battery_level),
    BT_GATT_CPF(&bas_cpf),
    BT_GATT_CCC(bas_ccc_cfg, bas_ccc_cfg_changed),    
);


void batery_service_init()
{
    bas_cpf.format = 4;
	bas_cpf.exponent = 1;
	bas_cpf.unit = 0;
	bas_cpf.name_space = 1;
	bas_cpf.description = 1;
}

void batery_notify(struct k_timer *timer)
{
    if (!battery_level_subscribed)
    {
        return;
    }
    //printk("BAS:> %d\n\r", battery_level);
    battery_level = getBatteryLevel();
    bt_gatt_notify(default_connection, &batery_level_svc.attrs[1], &battery_level, sizeof(battery_level));
}