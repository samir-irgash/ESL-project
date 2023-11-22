#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "nrfx_systick.h"
#include "nrfx_gpiote.h"
#include "my_gpio.h"
#include "app_timer.h"
#include "drv_rtc.h"
#include "nrf_drv_clock.h"
#include "my_switch.h"
#include "my_pwm.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define DEVICE_ID 1111
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

void init(void) {
    bsp_board_init(BSP_INIT_LEDS);
    my_gpio_config_sw1();
    my_gpio_config_leds();
    nrfx_systick_init();
    lfclk_request();
    app_timer_init();
    nrfx_gpiote_init();
    // ret_code_t ret_code = NRF_LOG_INIT(NULL);
    // APP_ERROR_CHECK(ret_code);
    // NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    init();
    my_switch_init();

    /* Toggle LEDs. */
    while (true)
    {
        for (size_t led_idx = 0; led_idx < LEDS_NUMBER; ++led_idx) {
            blink_context_t context = {
                .led_i = leds[led_idx],
                .duty_cycle_milli = DUTY_CYCLE_MIN_MILLI,
                .duty_cycle_step_milli = DUTY_CYCLE_STEP_MILLI,
                .next_exec_time_us = 0,
                .pwm_on = false,
                .pwm_state = BLINK_BEGIN,
                .take_step = true,
            };
            blink_n_times(&context, sequence[led_idx]);
        }
    }
}
