#include <stdint.h>
#include <stdbool.h>
#include "nrfx_systick.h"
#include "my_pwm.h"
#include "../my_gpio.h"

nrfx_systick_state_t pwm_state, current_state;
int duty_cycle_milli;
volatile bool pause;

#define PWM_FREQUENCY_HZ 1000
#define PWM_PERIOD_US 1000000 / PWM_FREQUENCY_HZ
#define DUTY_CYCLE_MAX_MILLI 1000
#define DUTY_CYCLE_MIN_MILLI 0
#define DUTY_CYCLE_STEP_MILLI 2
#define BLINK_DURATION_MS 100
#define STEP_US 100

int timeon_us(int duty_cycle_milli_arg) {
    return duty_cycle_milli_arg * PWM_PERIOD_US / 1000;
}

void my_pwm_turn_on(uint32_t led_i) {
        do {
            nrfx_systick_get(&current_state);
            my_gpio_set_led(led_i);
            while (!nrfx_systick_test(&current_state, timeon_us(duty_cycle_milli)));
            my_gpio_reset_led(led_i);
            while (!nrfx_systick_test(&current_state, PWM_PERIOD_US - timeon_us(duty_cycle_milli)));
        } while (pause);
}

void my_pwm_turn_off(uint32_t led_i) {
        do {
            nrfx_systick_get(&current_state);
            my_gpio_set_led(led_i);
            while (!nrfx_systick_test(&current_state, PWM_PERIOD_US - timeon_us(duty_cycle_milli)));
            my_gpio_reset_led(led_i);
            while (!nrfx_systick_test(&current_state, timeon_us(duty_cycle_milli)));
        } while (pause);
}

void my_pwm_blink(uint32_t led_i) {
    static bool blink_on;
    if (!nrfx_systick_test(&current_state, blink_on ? timeon_us(duty_cycle_milli) : PWM_PERIOD_US - timeon_us(duty_cycle_milli))) {
        if (blink_on) {
            my_pwm_turn_on(led_i);
        } else {
            my_pwm_turn_off(led_i);
        }
        duty_cycle_milli += DUTY_CYCLE_STEP_MILLI;
        if (duty_cycle_milli > DUTY_CYCLE_MAX_MILLI) {
            duty_cycle_milli = DUTY_CYCLE_MIN_MILLI;
            blink_on = !blink_on;
        }
        // Next process time
        nrfx_systick_get(&pwm_state);

    }
}