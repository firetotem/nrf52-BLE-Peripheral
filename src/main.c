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

#include <services/lux_service.h>


#include "main.h"

#define DEVICE_NAME         CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LENGTH  (sizeof(DEVICE_NAME) - 1)



/* Read lux value function callbak */
// static ssize_t readLux(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset)
// {
//   printk("Read request: data before = %d\n\r", sys_to_le16(*((uint16_t*)attr->user_data)));
  
//   lux_int16 = sys_to_le16((uint16_t)(lux.val1));

//   return bt_gatt_attr_read(conn, attr, buf, len, offset, &lux_int16, sizeof(lux_int16));
// }

// /* Read lux value function callbak */
// static ssize_t readDescription(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset)
// {
//   printk("Read request: data before = %s\n\r", attr->user_data);
  
//   //lux_int16 = sys_to_le16((uint16_t)(lux.val1));

//   return bt_gatt_attr_read(conn, attr, buf, len, offset, LUX_SENSOR, sizeof(LUX_SENSOR));
// }


// static void lux_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value)
// {
//   lux_notify = value == BT_GATT_CCC_NOTIFY;
//   if (lux_notify)
//     printk("Lux notify is enabled\n\r");
//   else
//     printk("Lux notify is disabled\n\r");
  
// }


// static ssize_t writeLux(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags)
// {
//   printk("len = %d, offset = %d\n\r", len, offset);

//   uint8_t *data = (uint8_t *)buf;

//   if (len > 0)
//   {
//     if (data[0] == LED_OP_ON)
//       gpio_pin_write(devGpio, LED1_GPIO_PIN, 1);
//     if (data[0] == LED_OP_OFF)
//       gpio_pin_write(devGpio, LED1_GPIO_PIN, 0);      
//   }
  


//   return len;
// }



/* GATT attributes definition */
// static struct bt_gatt_attr ess_attrs[] = {
//   BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),
//   BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE,
//                          BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
//                          BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
//                          readLux, writeLux, &lux_int16),
//   BT_GATT_DESCRIPTOR(BT_UUID_ES_MEASUREMENT, BT_GATT_PERM_READ, readDescription, NULL, LUX_SENSOR),
//   BT_GATT_CCC(lux_ccc_cfg, lux_ccc_cfg_changed),                      

// };






/* Defining gatt services */
// static struct bt_gatt_service ess_svc = BT_GATT_SERVICE(ess_attrs);

/* Connection callback function */
static void connected(struct bt_conn *conn, u8_t err)
{
  if (err) {
    printk("Connection failed (err %u)\n", err);
  } else {
    printk("Connected\n");
    led_set_state(LED_RED_PIN, false);
    led_set_state(LED_BLUE_PIN, true);
  }
}

/* Disconnection callback function */
static void disconnected(struct bt_conn *conn, u8_t reason)
{
  printk("Disconnected (reason %u)\n", reason);
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
};

/* Initialization bluetooth peripheral */ 
static void bt_ready(int err)
{
  if (err) {
    printk("%11d> Bluetooth init failed (err %d)\n", k_uptime_get(), err);
    return;
  }

  printk("%11d> Bluetooth initialized\n\r", k_uptime_get());

  lux_service_init();

  err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
  if (err) {
    printk("%11d> Advertising failed to start (err %d)\n\r", k_uptime_get(), err);
    return;
  }

  printk("%11d> Advertising successfully started\n\r", k_uptime_get());
}

void main(void)
{
    int err = bt_enable(bt_ready);
    if (err) {
      printk("Bluetooth init failed (err %d)\n", err);
      return;
    }

    led_init();
    led_set_state(LED_RED_PIN, true);

    bt_conn_cb_register(&conn_callbacks);
    
    
    //int err;
    struct sensor_value lux = {0};
    struct device *dev = device_get_binding(CONFIG_TSL2561_NAME);

     if (dev == NULL)
     {
       printk("Could not get TSL2561 device\n");
       return;
     }

    // if (sensor_sample_fetch(dev) < 0)
    // {
    //   printk("Error fetch data\n");
    // } else
    // {
    //   printk("Data TSL2561 fetched\n");
    // }

    // devGpio = device_get_binding(LED1_GPIO_CONTROLLER);
    // gpio_pin_configure(devGpio, LED1_GPIO_PIN, GPIO_DIR_OUT);
    // gpio_pin_write(devGpio, LED1_GPIO_PIN, 0);
    // uint8_t cnt = 0;

  /* Enable bluetooth peripheral */
  

  while (1) {
      k_sleep(MSEC_PER_SEC);
      //led_flash_state(true, false, 8);
    
     if (sensor_channel_get(dev, SENSOR_CHAN_LIGHT, &lux) < 0)
     {
       printk("Cannot read data TSL2561 \n");
       return;
     }

     if (lux.val1 < 100)
     {
        led_flash_state(false, true, 1);
     }

    // if (lux_notify)
    // {
    //   lux_int16 = sys_to_le16((uint16_t)(lux.val1));
      
    //   bt_gatt_notify(NULL, &ess_attrs[1], &lux_int16, sizeof(lux_int16));
    // }
    //gpio_pin_write(devGpio, LED1_GPIO_PIN, cnt++ % 2);
    /* CSC simulation */
    //if (csc_simulate) {
    //  csc_simulation();
    //}

    /* Battery level simulation */
    //bas_notify();
  }
}


/*
void main(void)
{



    struct sensor_value lux = {0};

    struct device *dev = device_get_binding(CONFIG_TSL2561_NAME);

    if (dev == NULL)
    {
      printk("Could not get TSL2561 device\n");
      return;
    }

    if (sensor_sample_fetch(dev) < 0)
    {
      printk("Error fetch data\n");
    } else
    {
      printk("Data TSL2561 fetched\n");
    }

    while (1)
    {
      if (sensor_channel_get(dev, SENSOR_CHAN_LIGHT, &lux) < 0)
      {
        printk("Cannot read data TSL2561 \n");
        return;
      }
*/
      /* display luminosity */
/*      printk("Luminosity: %d lx\n",lux.val1);

      k_sleep(2000);
    }
}
*/
