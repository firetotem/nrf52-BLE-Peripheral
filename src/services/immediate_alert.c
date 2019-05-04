#include <services/immediate_alert.h>

#define BT_UUID_IMMEDIATE_ALERT     BT_UUID_DECLARE_16(0x1802)
#define BT_UUID_ALERT_LEVEL         BT_UUID_DECLARE_16(0x2A06)

u8_t immediately_alert_level = 0;

BT_GATT_SERVICE_DEFINE(immediate_alert_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_IMMEDIATE_ALERT),
    BT_GATT_CHARACTERISTIC(BT_UUID_ALERT_LEVEL, BT_GATT_CHRC_WRITE_WITHOUT_RESP, BT_GATT_PERM_WRITE, NULL, write_immediate_alert_alert_level, NULL),
);


void immediate_alert_init()
{
	
}

ssize_t write_immediate_alert_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags)
{

    const u8_t *new_alert_level = buf;

    if(!len)
    {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }

    if(*new_alert_level >= 0 && *new_alert_level <= 2)
    {
        immediately_alert_level = *new_alert_level;
    } 
    else
	{
		return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
	}

    printk("IMMEDIATE Service:> write_immediate_alert_alert_level(%d)\n", immediately_alert_level);
}

u8_t get_immediate_alert_alert_level()
{
	return immediately_alert_level;
}