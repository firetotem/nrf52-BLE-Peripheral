#ifndef __THERMO_SERVICE_H__
#define __THERMO_SERVICE_H__

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <stdio.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>



typedef struct 
{
    /* Flags */
    u8_t flags;
    /* Therm value */
    u8_t value[4];

} __packed temperature_measurement_t;


void thermo_service_init();
void thermo_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value);
void thermo_measurement_indicate(struct k_timer *timer);
ssize_t	read_temperature_type(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset);

#endif