#ifndef __BLOOD_PRESSURE_SERVICE_H__
#define __BLOOD_PRESSURE_SERVICE_H__

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


enum {
    BPS_UNIT_FLAG       = 0x00,
    BPS_TIME_STAMP_FLAG,
    BPS_PULSE_RATE_FLAG,
    BPS_USER_ID_FLAG,
    BPS_MEASUREMENT_STATUS_FLAG
};


typedef struct
{
    union
    {
        uint8_t flags;
        struct {
            uint8_t bpu_flag    :1;
            uint8_t ts_flag     :1;
            uint8_t pr_flag     :1; 
            uint8_t uid_flag    :1;
            uint8_t ms_flag     :1;
            uint8_t             :3;
        } flags_fields;
    } flags;
    
    uint8_t bpm_systolic[2];
    uint8_t bpm_diastorlic[2];
    uint8_t bpm_mean_arterial[2];
    uint8_t pulse_rate[2];
    uint8_t user_id;
    uint16_t measurement_status;

} __packed blood_pressure_measurement_t;






void blood_pressure_service_init();
void bps_indicate(struct k_timer *timer);
ssize_t	read_blood_feature_type(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset);


#endif