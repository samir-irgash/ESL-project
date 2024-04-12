#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "ble.h"
#include "esl_ble_notify_indicate.h"
#include "esl_ble_service.h"

APP_TIMER_DEF(notifier_timer);
extern esl_ble_service_t m_esl_service;

static void handle_notifier_timeout(void *p_context) {
    if (m_esl_service.connection_handle != BLE_CONN_HANDLE_INVALID) {
        uint8_t type;
        ble_gatts_value_t cccd = {
            .p_value = &type,
            .len = sizeof(type),
            .offset = 0
        };
        sd_ble_gatts_value_get(m_esl_service.connection_handle, m_esl_service.notifier_handle.cccd_handle, &cccd);
        if (type == BLE_GATT_HVX_NOTIFICATION)
        {
            uint16_t len = sizeof(notifier_val);
            ble_gatts_hvx_params_t hvnotify_params = {
                .handle = m_esl_service.notifier_handle.value_handle,
                .type = BLE_GATT_HVX_NOTIFICATION,
                .offset = 0,
                .p_len = &len,
                .p_data = &notifier_val,
            };
            sd_ble_gatts_hvx(m_esl_service.connection_handle, &hvnotify_params);
        } else {
            NRF_LOG_INFO("Notifications are turned off on notifier\n");
        }

        sd_ble_gatts_value_get(m_esl_service.connection_handle, m_esl_service.indicator_handle.cccd_handle, &cccd);
        if (type == BLE_GATT_HVX_INDICATION) {
            uint16_t len = sizeof(indicator_val);
            ble_gatts_hvx_params_t hvindicate_params = {
                .handle = m_esl_service.indicator_handle.value_handle,
                .type = BLE_GATT_HVX_INDICATION,
                .offset = 0,
                .p_len = &len,
                .p_data = &indicator_val,
            };
            sd_ble_gatts_hvx(m_esl_service.connection_handle, &hvindicate_params);
        } else {
            NRF_LOG_INFO("Indications are turned off on indicator\n");
        }
    }

    if (notifier_val > 10) {
        notifier_val = 0;
    } else {
        notifier_val += 1;
    }

    if (indicator_val == 0) {
        indicator_val = 10;
    } else {
        indicator_val -= 1;
    }
    NRF_LOG_INFO("notifier: current value is %d", notifier_val);
    NRF_LOG_INFO("indicator: current value is %d", indicator_val);
}

void ble_notifier_init(void) {
    ret_code_t ret;

    ret = app_timer_create(&notifier_timer, APP_TIMER_MODE_REPEATED, handle_notifier_timeout);

    app_timer_start(notifier_timer, APP_TIMER_TICKS(TIMEOUT_MS), NULL);

    APP_ERROR_CHECK(ret);
}

