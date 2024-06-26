#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "boards.h"
#include "nrfx_systick.h"
#include "nrfx_gpiote.h"
#include "app_timer.h"
#include "drv_rtc.h"
#include "nrf_drv_clock.h"
#include "nrf_pwm.h"
#include "nrfx_pwm.h"
#include "nrf_delay.h"

#include "my_switch.h"
#include "my_pwm.h"
#include "my_gpio.h"
#include "my_flash.h"
#include "my_cli.h"
#include "my_ble_init.h"
#include "my_ble_updater.h"

#include "app_usbd.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"
#include "nrfx_pwm.h"
#include "nrf_pwm.h"

#define DEVICE_ID 1100
#define SMALL_DELAY_MS 500

const uint32_t leds[LEDS_NUMBER] = {LED_RED, LED_GREEN, LED_BLUE, LED_0};
const int sequence[LEDS_NUMBER] = {(DEVICE_ID/1000)%10, (DEVICE_ID/100)%10, (DEVICE_ID/10)%10, DEVICE_ID%10};
volatile bool pause;

/**@brief Function starting the internal LFCLK oscillator.
 *
 * @details This is needed by RTC1 which is used by the Application Timer
 *          (When SoftDevice is enabled the LFCLK is always running and this is not needed).
 */
void lfclk_request(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request(NULL);
}

void logs_init() {
    ret_code_t ret = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(ret);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
	LOG_BACKEND_USB_PROCESS();
}

void init(void) {
    lfclk_request();
    nrfx_systick_init();
    app_timer_init();
    nrfx_gpiote_init();
    bsp_board_init(BSP_INIT_LEDS);
    my_gpio_config_sw1();
    my_gpio_config_leds();
    my_switch_init();
}

/**
 * @brief Function for application main entry.
 */
int main(void) {
    init();
    my_pwm_init();

    logs_init();
    NRF_LOG_INFO("This is log\n");

    my_ble_init();

    my_flash_init();

    ret_code_t ret = my_cli_init();
    APP_ERROR_CHECK(ret);

    my_color_hsv_t default_hsv = {
        .hue = (DEVICE_ID % 100) * 255 / 100, 
        .saturation = 255, 
        .value = 255};

    if (!my_flash_data_check()) {
        context.hsv = default_hsv;
    } else {
        my_flash_read_hsv(&context.hsv);
    }


    while (true) {
        while (app_usbd_event_queue_process())
        {
            /* Nothing to do */
        }
        idle_state_handle();
    }
}
