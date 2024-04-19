#include <stdbool.h>
#include "nrfx_nvmc.h"
#include "my_color.h"
#include "my_flash.h"

#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"
#include "nrf_fstorage_nvmc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define START_ADDRESS 0x3e000
#define END_ADDRESS 0x3ffff

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);

NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    /* Set a handler for fstorage events. */
    .evt_handler = fstorage_evt_handler,

    /* These below are the boundaries of the flash space assigned to this instance of fstorage.
     * You must set these manually, even at runtime, before nrf_fstorage_init() is called.
     * The function nrf5_flash_end_addr_get() can be used to retrieve the last address on the
     * last page of flash available to write data. */
    .start_addr = START_ADDRESS,
    .end_addr   = END_ADDRESS,
};

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        NRF_LOG_INFO("--> Event received: ERROR while executing an fstorage operation.");
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        default:
            break;
    }
}

static void power_manage(void)
{
    (void) sd_app_evt_wait();
}

void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage)
{
    /* While fstorage is busy, sleep and wait for an event. */
    while (nrf_fstorage_is_busy(p_fstorage))
    {
        power_manage();
    }
}

void my_flash_init(void) {
    ret_code_t ret;
    nrf_fstorage_api_t * p_fs_api;
    NRF_LOG_INFO("Initializing softdevice implementation fstorage");

    p_fs_api = &nrf_fstorage_sd;
    ret = nrf_fstorage_init(&fstorage, p_fs_api, NULL);
    APP_ERROR_CHECK(ret);
}

void my_flash_read_hsv(my_color_hsv_t *p_hsv) {
    ret_code_t ret;
    writable_hsv data;

    ret = nrf_fstorage_read(&fstorage, START_ADDRESS, &data, sizeof(data));
    APP_ERROR_CHECK(ret);

    *p_hsv = data.padded.hsv;
}

void my_flash_write_hsv(my_color_hsv_t const *p_hsv) {
    ret_code_t ret;
    writable_hsv data;
    data.padded.padding = 0xFF;
    data.padded.hsv = *p_hsv;

    ret = nrf_fstorage_erase(&fstorage, START_ADDRESS, sizeof(data), NULL);
    APP_ERROR_CHECK(ret);

    ret = nrf_fstorage_write(&fstorage, START_ADDRESS, &data, sizeof(data), NULL);
    APP_ERROR_CHECK(ret);
}

bool my_flash_data_check(void) {
    ret_code_t ret;
    writable_hsv data;
    ret = nrf_fstorage_read(&fstorage, START_ADDRESS, &data, sizeof(data));
    APP_ERROR_CHECK(ret);

    return data.writable != 0xFFFFFFFF;
}