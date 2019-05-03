
#include <services/batery_service.h>

static u8_t battery_level = 100;
static u8_t battery_level_subscribed;

K_TIMER_DEFINE(notify_timer, batery_notify, NULL);

ssize_t	bas_read_fn(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &battery_level, sizeof(battery_level));
}

void bas_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    battery_level_subscribed = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;

    if (battery_level_subscribed)
    {
        k_timer_start(&notify_timer, K_MSEC(0), K_MSEC(1000));
    }
    else
    {
        k_timer_stop(&notify_timer);
    }    
}

static struct bt_gatt_ccc_cfg bas_ccc_cfg[BT_GATT_CCC_MAX] = {};

static struct bt_gatt_attr attrs[] = {
    BT_GATT_PRIMARY_SERVICE(BT_UUID_BAS),
    BT_GATT_CHARACTERISTIC(BT_UUID_BAS_BATTERY_LEVEL, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, bas_read_fn, NULL, NULL),
    BT_GATT_CCC(bas_ccc_cfg, bas_ccc_cfg_changed),
};

static struct bt_gatt_service batery_level_svc = BT_GATT_SERVICE(attrs);

void batery_service_init()
{
    bt_gatt_service_register(&batery_level_svc);
}

void batery_notify(struct k_timer *timer)
{
    if (!battery_level_subscribed)
    {
        return;
    }

    // todo: Read batery level
    bt_gatt_notify(NULL, &attrs[1], &battery_level, sizeof(battery_level));


}