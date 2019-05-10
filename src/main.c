#include <stdbool.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <device.h>
#include <sensor.h>
#include <gpio.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <hardware/led/led.h>
#include <hardware/lux_sensor/lux.h>
#include <hardware/adc/adc_bat.h>
#include <hardware/thermometr/thermometr.h>

#include <services/lux_service.h>
#include <services/batery_service.h>
#include <services/thermo_service.h>
#include <services/immediate_alert.h>
#include <services/blood_pressure_service.h>

#include "main.h"

#define DEVICE_NAME         CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LENGTH  (sizeof(DEVICE_NAME) - 1)

struct bt_conn *default_connection;

/* Connection callback function */
static void connected(struct bt_conn *conn, u8_t err)
{
  if (err) {
    printk("Connection failed (err %u)\n", err);
  } else {
    printk("Connected\n");
    default_connection = bt_conn_ref(conn);
    led_set_state(LED_RED_PIN, false);
    led_set_state(LED_BLUE_PIN, true);
  }
}

/* Disconnection callback function */
static void disconnected(struct bt_conn *conn, u8_t reason)
{
  printk("Disconnected (reason %u)\n", reason);
  bt_conn_unref(default_connection);
  default_connection = NULL;
  led_set_state(LED_RED_PIN, true);
  led_set_state(LED_BLUE_PIN, false);
}

/* Bluetooth connection callbacks struct */
static struct bt_conn_cb conn_callbacks = {
  .connected    = connected,
  .disconnected = disconnected,
};

/* Define advertising data packet */
static const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LENGTH),
  BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x0f, 0x18, /* BAS */
                                    0x09, 0x18, /* HTS */
                                    0x02, 0x18, /* IAS */
                                    0x10, 0x18),/* BPS */
};

/* Initialization bluetooth peripheral */ 
static void bt_ready(int err)
{
  if (err) {
    printk("Bluetooth init failed (err %d)\n", err);
    return;
  }

  printk("Bluetooth initialized\n\r");

  lux_service_init();
  batery_service_init();
  thermo_service_init();
  immediate_alert_init();
  blood_pressure_service_init();

  err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
  if (err) {
    printk("Advertising failed to start (err %d)\n\r", err);
    return;
  }

  printk("Advertising successfully started\n\r");
}

void main(void)
{
    int err = bt_enable(bt_ready);
    if (err) {
      printk("Bluetooth init failed (err %d)\n", err);
      return;
    }

    led_init();
    lux_sensor_init();
    adc_bat_init();
    thermometr_init();

    led_set_state(LED_RED_PIN, true);

    bt_conn_cb_register(&conn_callbacks);
 

  /* Enable bluetooth peripheral */

  while (1) {
      k_sleep(MSEC_PER_SEC); 
      u8_t alert_level = get_immediate_alert_alert_level();
      uint16_t lux = getLuxValue();
      
      if (lux < 300)
      {
        switch (alert_level)
        {
          case ALERT_LEVEL_MID_ALERT:
              led_flash_state(true, false, 1);
              break;
          case ALERT_LEVEL_HIGH_ALERT:
              led_set_state(LED_RED_PIN, true);
              break;
          case ALERT_LEVEL_NO_ALERT:
              led_set_state(LED_RED_PIN, false);
          default:
          break;
        }
      }      
  }
}