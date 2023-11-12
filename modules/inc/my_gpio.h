#ifndef __MY_NRF_GPIO
#define __MY_NRF_GPIO

#include <stdint.h>

#define SW2_PIN NRF_GPIO_PIN_MAP(1, 6)
#define LED_0 NRF_GPIO_PIN_MAP(0, 6)
#define LED_RED NRF_GPIO_PIN_MAP(0, 8)
#define LED_GREEN NRF_GPIO_PIN_MAP(1, 9)
#define LED_BLUE NRF_GPIO_PIN_MAP(0, 12)

#define NRF_GPIO_SW1_PULL NRF_GPIO_PIN_PULLUP

#define MY_GPIO_LED_SET 0
#define MY_GPIO_LED_RESET 1
#define MY_GPIO_SW2_PRESSED 0
#define MY_GPIO_SW2_RELEASED 1

void my_gpio_config_sw2(void);

void my_gpio_config_leds(void);

void my_gpio_set_led(uint32_t led_idx);

void my_gpio_reset_led(uint32_t led_idx);

bool my_gpio_read_pin(uint32_t pin);


#endif // __MY_NRF_GPIO