#include <string.h>
#include <stdbool.h>
#include "nrf_gpio.h"
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "nrfx_systick.h"
#include "my_gpio.h"

#define DEVICE_ID 1111
#define SMALL_DELAY_MS 500

#define PWM_FREQUENCY_HZ 1000
#define PWM_PERIOD_US 1000000 / PWM_FREQUENCY_HZ
#define BLINK_DURATION_MS 100
#define STEP_US 100

const uint32_t leds[LEDS_NUMBER] = {LED_RED, LED_GREEN, LED_BLUE, LED_0};

int timeon_us(int duty_cycle_milli) {
    return duty_cycle_milli * PWM_PERIOD_US / 1000;
}

void blink_n_times(int n, uint32_t led_idx) {
    
    for (size_t i = 0; i < n; ++i) {
        nrfx_systick_state_t current_state;
        for (size_t duty_cycle_milli = 0; duty_cycle_milli <= 1000; duty_cycle_milli += 2) {
            while (1) {
                nrfx_systick_get(&current_state);
                led_set(led_idx);
                while (!nrfx_systick_test(&current_state, timeon_us(duty_cycle_milli)));
                led_reset(led_idx);
                while (!nrfx_systick_test(&current_state, PWM_PERIOD_US - timeon_us(duty_cycle_milli)));
                if (read(SW1_PIN) == SW1_PRESSED) {
                    break;
                }
            }
        }
        
        for (size_t duty_cycle_milli = 0; duty_cycle_milli <= 1000; duty_cycle_milli += 2) {
            while (1) {
                nrfx_systick_get(&current_state);
                led_set(led_idx);
                while (!nrfx_systick_test(&current_state, PWM_PERIOD_US - timeon_us(duty_cycle_milli)));
                led_reset(led_idx);
                while (!nrfx_systick_test(&current_state, timeon_us(duty_cycle_milli)));
                if (read(SW1_PIN) == SW1_PRESSED) {
                    break;
                }
            };
        }
        led_reset(led_idx);
        nrf_delay_ms(100);
    }
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);
    config_sw1();
    config_leds();
    nrfx_systick_init();

    /* Toggle LEDs. */
    while (true)
    {
        size_t i;
        for (i = 0; i < LEDS_NUMBER; ++i) {
            blink_device_id(leds[i]);
        }
    }
}