#ifndef BLE_UPDATER_H__
#define BLE_UPDATER_H__

#include "ble.h"

void my_ble_notify_all(my_color_rgb_t *rgb);
void my_ble_update_by_handle(ble_uuid_t uuid, uint16_t handle, uint8_t value);


#endif // BLE_UPDATER_H__