#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "nrfx_systick.h"

#define DEVICE_ID 1111

#define SMALL_DELAY_MS 500

#define SW2_PIN NRF_GPIO_PIN_MAP(1, 6)
#define LED_0 NRF_GPIO_PIN_MAP(0, 6)
#define LED_RED NRF_GPIO_PIN_MAP(0, 8)
#define LED_GREEN NRF_GPIO_PIN_MAP(1, 9)
#define LED_BLUE NRF_GPIO_PIN_MAP(0, 12)

#undef LEDS_NUMBER
#define LEDS_NUMBER 4

#define NRF_GPIO_SW2_PULL NRF_GPIO_PIN_PULLUP

#define LED_SET 0
#define LED_RESET 1
#define SW2_PRESSED 0
#define SW2_RELEASED 1

#define PWM_FREQUENCY_HZ 1000
#define PWM_PERIOD_US 1000000 / PWM_FREQUENCY_HZ
#define BLINK_DURATION_MS 100
#define STEP_US 100

const uint32_t leds[LEDS_NUMBER] = {LED_RED, LED_GREEN, LED_BLUE, LED_0};

void config_sw2(void) {
    nrf_gpio_cfg_input(SW2_PIN, NRF_GPIO_SW2_PULL);
}

void config_leds(void) {
    nrf_gpio_cfg_output(LED_RED);
    nrf_gpio_cfg_output(LED_GREEN);
    nrf_gpio_cfg_output(LED_BLUE);
}

void led_set(uint32_t led_idx) {
    nrf_gpio_pin_write(led_idx, LED_SET);
}

void led_reset(uint32_t led_idx) {
    nrf_gpio_pin_write(led_idx, LED_RESET);
}

bool read(uint32_t pin) {
    return nrf_gpio_pin_read(pin);
}

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
                if (read(SW2_PIN) == SW2_PRESSED) {
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
                if (read(SW2_PIN) == SW2_PRESSED) {
                    break;
                }
            };
        }
        led_reset(led_idx);
        nrf_delay_ms(100);
    }
}

void blink_device_id(uint32_t led_idx) {
    blink_n_times((DEVICE_ID/1000)%10, led_idx);
    nrf_delay_ms(SMALL_DELAY_MS);
    
    blink_n_times((DEVICE_ID/100)%10, led_idx);
    nrf_delay_ms(SMALL_DELAY_MS);
    
    blink_n_times((DEVICE_ID/10)%10, led_idx);
    nrf_delay_ms(SMALL_DELAY_MS);

    blink_n_times(DEVICE_ID%10, led_idx);
    nrf_delay_ms(SMALL_DELAY_MS);
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);
    config_sw2();
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