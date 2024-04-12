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

#include "esl_ble_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

uint8_t notifier_val;
uint8_t indicator_val;

static ret_code_t esl_ble_add_char_random(esl_ble_service_t *service);
static ret_code_t esl_ble_add_char_notifier(esl_ble_service_t *service);
static ret_code_t esl_ble_add_char_indicator(esl_ble_service_t *service);

ret_code_t esl_ble_service_init(esl_ble_service_t *service)
{
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t service_uuid;
    // TODO: 3. Add service UUIDs to the BLE stack table using `sd_ble_uuid_vs_add`
    ble_uuid128_t base_uuid = {
        .uuid128 = ESL_BASE_UUID
    };
    service_uuid.uuid = ESL_SERVICE_UUID;
    ret = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    service->uuid = service_uuid;
    APP_ERROR_CHECK(ret);

    ret = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                          &service_uuid,
                                          &service->service_handle);
    APP_ERROR_CHECK(ret);

    ret = esl_ble_add_char_random(service);
    APP_ERROR_CHECK(ret);
    ret = esl_ble_add_char_notifier(service);
    APP_ERROR_CHECK(ret);
    ret = esl_ble_add_char_indicator(service);
    return ret;
}

static ret_code_t esl_ble_add_char_indicator(esl_ble_service_t *service) {
    uint8_t char_desc[] = ESL_GATT_INDICATE_DESC;
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t indicator_char_uuid = {
        .uuid = ESL_GATT_INDICATE_UUID,
        .type = service->uuid.type
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    ble_gatts_attr_t indicator_attr_val = {
        .p_uuid = &indicator_char_uuid,
        .p_attr_md = &attr_md,
        .init_len = sizeof(indicator_val),
        .max_len = sizeof(indicator_val),
    };

    ble_gatts_char_md_t indicator_char_md = {
        .char_props.indicate = 1,
        .p_char_user_desc = char_desc,
        .char_user_desc_size = sizeof(char_desc),
        .char_user_desc_max_size = sizeof(char_desc),
        .p_cccd_md = &cccd_md,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &indicator_char_md,
                                          &indicator_attr_val,
                                          &service->indicator_handle);
    return ret;
}

static ret_code_t esl_ble_add_char_notifier(esl_ble_service_t *service) {
    uint8_t char_desc[] = ESL_GATT_NOTIFY_DESC;
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t notifier_char_uuid = {
        .uuid = ESL_GATT_NOTIFY_UUID,
        .type = service->uuid.type
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    ble_gatts_attr_t notifier_attr_val = {
        .p_uuid = &notifier_char_uuid,
        .p_attr_md = &attr_md,
        .init_len = sizeof(notifier_val),
        .max_len = sizeof(notifier_val),
    };

    ble_gatts_char_md_t notifier_char_md = {
        .char_props.notify = 1,
        .p_char_user_desc = char_desc,
        .char_user_desc_size = sizeof(char_desc),
        .char_user_desc_max_size = sizeof(char_desc),
        .p_cccd_md = &cccd_md,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &notifier_char_md,
                                          &notifier_attr_val,
                                          &service->notifier_handle);

    return ret;
}

static ret_code_t esl_ble_add_char_random(esl_ble_service_t *service)
{
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t characteristic_uuid = {
        .uuid = ESL_GATT_CHAR_1_UUID,
        .type = service->uuid.type
    };

    ble_gatts_char_md_t char_md = {
        .char_props.read = 1,
        .char_props.write = 1
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t attr_char_value = {
        .p_uuid = &characteristic_uuid,
        .p_attr_md = &attr_md,
        .init_offs = 0,
        .init_len = 5,
        .max_len = 5,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &char_md,
                                          &attr_char_value,
                                          &service->characteristic_handle);
    return ret;
}