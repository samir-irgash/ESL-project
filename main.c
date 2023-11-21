#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "nrfx_systick.h"
#include "nrfx_gpiote.h"
#include "my_gpio.h"
#include "app_timer.h"
#include "drv_rtc.h"
#include "nrf_drv_clock.h"
#include "my_switch.h"
#include "my_pwm.h"

#define DEVICE_ID 1111
#define SMALL_DELAY_MS 500

const uint32_t leds[LEDS_NUMBER] = {LED_RED, LED_GREEN, LED_BLUE, LED_0};
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

void blink_n_times(uint32_t led_idx, int n) {
    
    for (size_t i = 0; i < n; ++i) {
        my_pwm_blink(led_idx);
        nrf_delay_ms(100);
    }
}

void initialize(void) {
    bsp_board_init(BSP_INIT_LEDS);
    my_gpio_config_sw1();
    my_gpio_config_leds();
    nrfx_systick_init();
    lfclk_request();
    app_timer_init();
    nrfx_gpiote_init();
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    initialize();
    my_switch_init();

    /* Toggle LEDs. */
    while (true)
    {
        blink_n_times(leds[0], (DEVICE_ID/1000)%10);
        nrf_delay_ms(SMALL_DELAY_MS);
        blink_n_times(leds[1], (DEVICE_ID/100)%10);
        nrf_delay_ms(SMALL_DELAY_MS);
        blink_n_times(leds[2], (DEVICE_ID/10)%10);
        nrf_delay_ms(SMALL_DELAY_MS);
        blink_n_times(leds[3], DEVICE_ID%10);
        nrf_delay_ms(SMALL_DELAY_MS);
    }
}
