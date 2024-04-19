#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "ble.h"
#include "my_led_service.h"
#include "my_ble_updater.h"
#include "my_pwm.h"
#include "my_flash.h"

extern esl_ble_service_t m_esl_service;

static void my_ble_notify_value(uint16_t conn_handle, ble_gatts_char_handles_t char_handle, uint8_t *value) {
    NRF_LOG_INFO("Sending value %d to characteristic %d", *value, char_handle.value_handle);
    uint16_t len = sizeof(*value);
    ble_gatts_hvx_params_t params = {
        .handle = char_handle.value_handle,
        .type = BLE_GATT_HVX_NOTIFICATION,
        .offset = 0,
        .p_len = &len,
        .p_data = value,
    };
    sd_ble_gatts_hvx(conn_handle, &params);
}

void my_ble_notify_all(my_color_rgb_t *rgb) {
    if (m_esl_service.connection_handle == BLE_CONN_HANDLE_INVALID) {
        NRF_LOG_INFO("No BLE connection yet, rgb data is not sent");
        return ;
    }
    my_ble_notify_value(m_esl_service.connection_handle, 
                        m_esl_service.char_red_handle, 
                        &rgb->red);
                    
    my_ble_notify_value(m_esl_service.connection_handle, 
                        m_esl_service.char_green_handle, 
                        &rgb->green);

    my_ble_notify_value(m_esl_service.connection_handle, 
                        m_esl_service.char_blue_handle, 
                        &rgb->blue);
}

void my_ble_update_by_handle(ble_uuid_t uuid, uint16_t handle, uint8_t value) {
        if (uuid.uuid == ESL_GATT_RED_UUID && handle == m_esl_service.char_red_handle.value_handle) {
            NRF_LOG_INFO("Red value updated through characteristic");
            context.rgb.red = value;
            my_color_rgb2hsv(&context.rgb, &context.hsv);
            my_flash_write_hsv(&context.hsv);
        } else if (uuid.uuid == ESL_GATT_GREEN_UUID && handle == m_esl_service.char_green_handle.value_handle) {
            NRF_LOG_INFO("Green value updated through characteristic");
            context.rgb.green = value;
            my_color_rgb2hsv(&context.rgb, &context.hsv);
            my_flash_write_hsv(&context.hsv);
        }
        else if (uuid.uuid == ESL_GATT_BLUE_UUID && handle == m_esl_service.char_blue_handle.value_handle) {
            NRF_LOG_INFO("Blue value updated through characteristic");
            context.rgb.blue = value;
            my_color_rgb2hsv(&context.rgb, &context.hsv);
            my_flash_write_hsv(&context.hsv);
        } else { NRF_LOG_INFO("Invalid characteristic handle"); }
}