
#include <services/lux_service.h>
#include <hardware/lux_sensor/lux.h>

#define BT_UUID_LUX_SERVICE     BT_UUID_DECLARE_128(0x6c, 0x12, 0x48, 0xb8, 0x6c, 0xb0, 0x11, 0xe9, 0xa9, 0x23, 0x16, 0x81, 0xbe, 0x66, 0x3d, 0x3e)
#define BT_UUID_LUX_VALUE       BT_UUID_DECLARE_128(0xfd, 0xc1, 0x1e, 0x88, 0x6c, 0xce, 0x11, 0xe9, 0xa9, 0x23, 0x16, 0x81, 0xbe, 0x66, 0x3d, 0x3e)
#define BT_UUID_LUX_DESCRIPTOR  BT_UUID_DECLARE_128(0x3e, 0x3d, 0x66, 0xbe, 0x81, 0x16, 0x23, 0xa9, 0xe9, 0x11, 0xda, 0x6c, 0x8c, 0xdc, 0x18, 0xb5)


u16_t lux_value = 0;


BT_GATT_SERVICE_DEFINE(lux_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_LUX_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_LUX_VALUE, BT_GATT_CHRC_READ, BT_GATT_PERM_READ, read_client_lux, NULL, NULL),
    BT_GATT_DESCRIPTOR(BT_UUID_LUX_DESCRIPTOR, BT_GATT_PERM_READ, read_lux_descriptor, NULL, NULL),
);




void lux_service_init()
{    
    //printk("Lux service initialized\n\r");
}


ssize_t	read_client_lux(struct bt_conn *conn, 
                         const struct bt_gatt_attr *attr,
					     void *buf, 
                         u16_t len,
					     u16_t offset)
{   
    prepare_data_for_srv(getLuxValue());
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &lux_value, sizeof(lux_value));
}


ssize_t	read_lux_descriptor(struct bt_conn *conn,
					        const struct bt_gatt_attr *attr,
					        void *buf, u16_t len,
					        u16_t offset)
{
    u8_t description[] = {'t', 's', 'l', '2', '5', '6', '1'};
    
    return bt_gatt_attr_read(conn, attr, buf, len, offset, description, sizeof(description));
}


void prepare_data_for_srv(uint16_t lux)
{
    lux_value = __bswap_16(lux);
}
