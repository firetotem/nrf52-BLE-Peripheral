#include <services/blood_pressure_service.h>
#include <hardware/bp_simulator/bp_simulator.h>

#define BT_UUID_BPS                         BT_UUID_DECLARE_16(0x1810)
#define BT_UUID_BLOOD_PRESSURE_MEASUREMENT  BT_UUID_DECLARE_16(0x2A35)
#define BT_UUID_BLOOD_PRESSURE_FEATURE      BT_UUID_DECLARE_16(0x2A49)

static u8_t bps_subscribed;
static blood_pressure_measurement_t bps_measurement = {0};
static struct bt_gatt_indicate_params indicate_param = {0};
static uint16_t bps_feature = 0xf;
extern struct bt_conn *default_connection;

/* Kernel's variables */
static struct bt_gatt_ccc_cfg bps_ccc_cfg[BT_GATT_CCC_MAX] = {};


K_TIMER_DEFINE(bps_timer_indicate, bps_indicate, NULL);


void bps_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    bps_subscribed = (value == BT_GATT_CCC_INDICATE) ? 1 : 0;
    
    bps_subscribed ? printk("BPS Service:> indicate enabled\n\r") : printk("BPS Service:> indicate disabled\n\r");
    
    if (bps_subscribed)
    {
        k_timer_start(&bps_timer_indicate, K_MSEC(0), K_MSEC(1000));
    }
    else
    {
        k_timer_stop(&bps_timer_indicate);
    }    
}


BT_GATT_SERVICE_DEFINE(bps_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_BPS),
    BT_GATT_CHARACTERISTIC(BT_UUID_BLOOD_PRESSURE_MEASUREMENT, BT_GATT_CHRC_INDICATE, BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(bps_ccc_cfg, bps_ccc_cfg_changed), 
    BT_GATT_CHARACTERISTIC(BT_UUID_BLOOD_PRESSURE_FEATURE, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_blood_feature_type, NULL, &bps_feature),
);

ssize_t	read_blood_feature_type(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset)
{
    
    printk("BPS Service:> read request\n\r");

    return bt_gatt_attr_read(conn, attr, buf, len, offset, &bps_feature, sizeof(bps_feature));
}

void blood_pressure_service_init()
{
    bps_measurement.flags.flags = (1 << BPS_PULSE_RATE_FLAG) | (1 << BPS_USER_ID_FLAG) | (1 << BPS_MEASUREMENT_STATUS_FLAG);    
}

static void uint16_to_sfloat(uint8_t *dest, uint16_t value)
{
    dest[0] = value;
    dest[1] = (value >> 12);
    dest[1] &= 0x0F;
    dest[1] |= (1 << 12);
}

static void bps_measurement_indicate_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, u8_t err)
{
	if (err != 0)
	{
		printk("BPS Service:> Indication failed: %d\n", err);
	}
}

void bps_indicate(struct k_timer *timer)
{
    if (!bps_subscribed)
    {
        return;
    }
     
    bp_simulate();

    uint16_to_sfloat(bps_measurement.bpm_systolic, get_systolic());
    uint16_to_sfloat(bps_measurement.bpm_diastorlic, get_diastolyc());
    uint16_to_sfloat(bps_measurement.bpm_mean_arterial, get_mean_arterial());
    uint16_to_sfloat(bps_measurement.pulse_rate, get_pulse_rate());
    bps_measurement.user_id = 0;
    bps_measurement.measurement_status = 0;
    
    indicate_param.attr = &bps_svc.attrs[2];
    indicate_param.data = &bps_measurement;
    indicate_param.len  = sizeof(bps_measurement);
    indicate_param.func = bps_measurement_indicate_cb;

    bt_gatt_indicate(default_connection, &indicate_param);

    //printk("BPS Service:> bps systolic = %d\n\r", bps_measurement.bpm_systolic[0]);
}