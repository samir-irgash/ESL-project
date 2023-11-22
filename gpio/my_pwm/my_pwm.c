#include <stdint.h>
#include <stdbool.h>
#include "nrfx_systick.h"
#include "my_pwm.h"
#include "../my_gpio.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

volatile bool pause;

int time_us(int duty_cycle_milli_arg) {
    return (duty_cycle_milli_arg * PWM_PERIOD_US) / 1000;
}

void blink_n_times(blink_context_t *context, int n) {
    for (size_t i = 0; i < n; ++i) {
        while (context->pwm_state != BLINK_END) {
            bool step = my_pwm_blink(context);

            if (step && !pause) {
                my_pwm_step(context);
            }
        }
    }
}

bool my_pwm_blink(blink_context_t *context) {
    if (nrfx_systick_test(&context->pwm_systick_state, context->next_exec_time_us)) {
        nrfx_systick_get(&context->pwm_systick_state);
        context->pwm_on = !context->pwm_on;
        if (context->pwm_on) {
            my_gpio_set_led(context->led_i);
            context->next_exec_time_us = time_us(context->duty_cycle_milli);
            return false;
        } 
        else {
            my_gpio_reset_led(context->led_i);
            context->next_exec_time_us = PWM_PERIOD_US - time_us(context->duty_cycle_milli);
            return true;
        }
    }
    return false;
}

void my_pwm_step(blink_context_t *context) {
    context->duty_cycle_milli += context->duty_cycle_step_milli;

    if (context->duty_cycle_milli == DUTY_CYCLE_MAX_MILLI) {
        context->duty_cycle_step_milli = -context->duty_cycle_step_milli;
    }
    if (context->duty_cycle_milli == DUTY_CYCLE_MIN_MILLI) {
        context->pwm_state = BLINK_END;
    }
}