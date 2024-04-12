/**
 * Copyright 2022 Evgeniy Morozov
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE
*/

#ifndef ESL_SERVICE_H__
#define ESL_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESL_BASE_UUID {0xE5, 0x79, 0x0A, 0xB8, 0x38, 0x75, /* - */ 0xD1, 0x88, /* - */ 0xC7, 0x49, /* - */ 0x4A, 0x54, /* - */ 0x00, 0x00, 0x51, 0x50}

#define ESL_SERVICE_UUID 0x183A // Insulin Delivery Service
#define ESL_GATT_CHAR_1_UUID 0x1010
#define ESL_GATT_NOTIFY_UUID   0x1011
#define ESL_GATT_NOTIFY_DESC "Notifier"
#define ESL_GATT_INDICATE_UUID 0x1012
#define ESL_GATT_INDICATE_DESC "Indicator"

typedef struct
{
    ble_uuid_t uuid;
    uint16_t service_handle;
    uint16_t connection_handle;
    ble_gatts_char_handles_t characteristic_handle;
    ble_gatts_char_handles_t notifier_handle;
    ble_gatts_char_handles_t indicator_handle;
} esl_ble_service_t;

extern uint8_t notifier_val;
extern uint8_t indicator_val;

ret_code_t esl_ble_service_init(esl_ble_service_t *service);

#endif /* ESL_SERVICE_H__ */