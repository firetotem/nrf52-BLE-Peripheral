#ifndef __IMMEDIATE_ALERT_H__
#define __IMMEDIATE_ALERT_H__

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


enum alert_level
{
    ALERT_LEVEL_NO_ALERT = 0x00,
    ALERT_LEVEL_MID_ALERT,
    ALERT_LEVEL_HIGH_ALERT
};


void immediate_alert_init();
u8_t get_immediate_alert_alert_level();
ssize_t write_immediate_alert_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags);

#endif