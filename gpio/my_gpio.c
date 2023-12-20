#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "my_gpio.h"

void my_gpio_config_sw1(void) {
    nrf_gpio_cfg_input(SW1_PIN, NRF_GPIO_SW1_PULL);
}

void my_gpio_config_leds(void) {
    nrf_gpio_cfg_output(LED_RED);
    nrf_gpio_cfg_output(LED_GREEN);
    nrf_gpio_cfg_output(LED_BLUE);
}

void my_gpio_set_led(uint32_t led_idx) {
    nrf_gpio_pin_write(led_idx, MY_GPIO_LED_SET);
}

void my_gpio_reset_led(uint32_t led_idx) {
    nrf_gpio_pin_write(led_idx, MY_GPIO_LED_RESET);
}

void my_gpio_toggle_led(uint32_t led_idx) {
    nrf_gpio_pin_toggle(led_idx);
}

bool my_gpio_read_pin(uint32_t pin) {
    return nrf_gpio_pin_read(pin);
}

