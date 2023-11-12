#include <stdbool.h>
#include <stdint.h>
#include "my_gpio.h"

void my_gpio_config_sw2(void) {
    nrf_gpio_cfg_input(SW2_PIN, NRF_GPIO_SW2_PULL);
}

void my_gpio_config_leds(void) {
    nrf_gpio_cfg_output(LED_RED);
    nrf_gpio_cfg_output(LED_GREEN);
    nrf_gpio_cfg_output(LED_BLUE);
}

void my_gpio_set_led(uint32_t led_idx) {
    nrf_gpio_pin_write(led_idx, LED_SET);
}

void my_gpio_reset_led(uint32_t led_idx) {
    nrf_gpio_pin_write(led_idx, LED_RESET);
}

bool my_gpio_read_pin(uint32_t pin) {
    return nrf_gpio_pin_read(pin);
}