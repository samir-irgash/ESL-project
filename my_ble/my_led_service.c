#include "my_led_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

static ret_code_t esl_ble_add_char_red(esl_ble_service_t *service);
static ret_code_t esl_ble_add_char_green(esl_ble_service_t *service);
static ret_code_t esl_ble_add_char_blue(esl_ble_service_t *service);

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

    ret = esl_ble_add_char_red(service);
    APP_ERROR_CHECK(ret);
    ret = esl_ble_add_char_blue(service);
    APP_ERROR_CHECK(ret);
    ret = esl_ble_add_char_green(service);
    return ret;
}

static ret_code_t esl_ble_add_char_red(esl_ble_service_t *service) {
    uint8_t char_desc[] = ESL_GATT_DESC_RED;
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t indicator_char_uuid = {
        .uuid = ESL_GATT_RED_UUID,
        .type = service->uuid.type
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    ble_gatts_attr_t indicator_attr_val = {
        .p_uuid = &indicator_char_uuid,
        .p_attr_md = &attr_md,
        .init_len = sizeof(context.rgb.red),
        .max_len = sizeof(context.rgb.red),
    };

    ble_gatts_char_md_t indicator_char_md = {
        .char_props.notify = 1,
        .char_props.read = 1,
        .char_props.write = 1,
        .p_char_user_desc = char_desc,
        .char_user_desc_size = sizeof(char_desc),
        .char_user_desc_max_size = sizeof(char_desc),
        .p_cccd_md = &cccd_md,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &indicator_char_md,
                                          &indicator_attr_val,
                                          &service->char_red_handle);
    return ret;
}

static ret_code_t esl_ble_add_char_green(esl_ble_service_t *service) {
    uint8_t char_desc[] = ESL_GATT_DESC_GREEN;
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t notifier_char_uuid = {
        .uuid = ESL_GATT_GREEN_UUID,
        .type = service->uuid.type
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    ble_gatts_attr_t notifier_attr_val = {
        .p_uuid = &notifier_char_uuid,
        .p_attr_md = &attr_md,
        .init_len = sizeof(context.rgb.green),
        .max_len = sizeof(context.rgb.green),
    };

    ble_gatts_char_md_t notifier_char_md = {
        .char_props.notify = 1,
        .char_props.read = 1,
        .char_props.write = 1,
        .p_char_user_desc = char_desc,
        .char_user_desc_size = sizeof(char_desc),
        .char_user_desc_max_size = sizeof(char_desc),
        .p_cccd_md = &cccd_md,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &notifier_char_md,
                                          &notifier_attr_val,
                                          &service->char_green_handle);

    return ret;
}

static ret_code_t esl_ble_add_char_blue(esl_ble_service_t *service) {
    uint8_t char_desc[] = ESL_GATT_DESC_BLUE;
    ret_code_t ret = NRF_SUCCESS;

    ble_uuid_t notifier_char_uuid = {
        .uuid = ESL_GATT_BLUE_UUID,
        .type = service->uuid.type
    };

    ble_gatts_attr_md_t attr_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_md_t cccd_md = {
        .vloc = BLE_GATTS_VLOC_STACK,
    };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    ble_gatts_attr_t notifier_attr_val = {
        .p_uuid = &notifier_char_uuid,
        .p_attr_md = &attr_md,
        .init_len = sizeof(context.rgb.blue),
        .max_len = sizeof(context.rgb.blue),
    };

    ble_gatts_char_md_t notifier_char_md = {
        .char_props.notify = 1,
        .char_props.read = 1,
        .char_props.write = 1,
        .p_char_user_desc = char_desc,
        .char_user_desc_size = sizeof(char_desc),
        .char_user_desc_max_size = sizeof(char_desc),
        .p_cccd_md = &cccd_md,
    };

    ret = sd_ble_gatts_characteristic_add(service->service_handle,
                                          &notifier_char_md,
                                          &notifier_attr_val,
                                          &service->char_blue_handle);

    return ret;
}