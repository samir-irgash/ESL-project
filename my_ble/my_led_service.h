#ifndef ESL_SERVICE_H__
#define ESL_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"
#include "my_pwm.h"

#define ESL_BASE_UUID {0xE5, 0x79, 0x0A, 0xB8, 0x38, 0x75, /* - */ 0xD1, 0x88, /* - */ 0xC7, 0x49, /* - */ 0x4A, 0x54, /* - */ 0x00, 0x00, 0x51, 0x50}

#define ESL_SERVICE_UUID 0x183A // Insulin Delivery Service
#define ESL_GATT_CHAR_1_UUID 0x1010
#define ESL_GATT_RED_UUID   0x1011
#define ESL_GATT_DESC_RED "RED"
#define ESL_GATT_GREEN_UUID 0x1012
#define ESL_GATT_DESC_GREEN "GREEN"
#define ESL_GATT_BLUE_UUID 0x1013
#define ESL_GATT_DESC_BLUE "BLUE"

typedef struct
{
    ble_uuid_t uuid;
    uint16_t service_handle;
    uint16_t connection_handle;
    ble_gatts_char_handles_t char_red_handle;
    ble_gatts_char_handles_t char_blue_handle;
    ble_gatts_char_handles_t char_green_handle;
} esl_ble_service_t;

ret_code_t esl_ble_service_init(esl_ble_service_t *service);

#endif /* ESL_SERVICE_H__ */