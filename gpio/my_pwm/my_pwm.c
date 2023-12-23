#include <stdint.h>
#include <stdbool.h>
#include "nrfx_systick.h"
#include "my_pwm.h"
#include "../my_gpio.h"
#include "nrfx_pwm.h"
#include "nrf_pwm.h"
#include "my_color.h"
#include "nrf_delay.h"
#include "my_switch.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

/*
    | Input state         | Yellow LED    | Duty cycle    |
    |---------------------|---------------|---------------|
    | INPUT_NO            | Turned off    | 0 / 255       |
    | INPUT_HUE           | Slow blinking | variable      |
    | INPUT_SATURATION    | Fast blinking | variable      |   
    | INPUT_VALUE         | Turned on     | 255           |
    |---------------------|---------------|---------------|
*/

my_pwm_context_t context;
nrfx_pwm_t pwm = NRFX_PWM_INSTANCE(0);
nrf_pwm_values_individual_t seq_values[] = {{0, 0, 0, 0}};
nrf_pwm_sequence_t seq = {
    .values.p_individual = seq_values,
    .length = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats = 0,
    .end_delay = 0,
};

my_pwm_input_mode my_pwm_next_mode(my_pwm_input_mode mode) {
    switch (mode) {
        case INPUT_NO:
            return INPUT_HUE;
        case INPUT_HUE:
            return INPUT_SATURATION;
        case INPUT_SATURATION:
            return INPUT_VALUE;
        case INPUT_VALUE:
            return INPUT_NO;
    }
    return INPUT_NO;
}

void handler(nrfx_pwm_evt_type_t event) {
    update_yellow(&context);

    if (switch_state == SWITCH_PRESSED) {
        switch (context.input_mode) {
            case INPUT_NO:
                return;
            case INPUT_HUE:
                my_pwm_update_hue(&context);
                break;
            case INPUT_SATURATION:
                my_pwm_update_saturation(&context);
                break;
            case INPUT_VALUE:
                my_pwm_update_value(&context);
                break;
        }
    }
    my_color_hsv2rgb(&(context.hsv), &(context.rgb));

    NRF_LOG_INFO("HSV: %d, %d, %d\nRGB: %d, %d, %d,", 
                    context.hsv.hue, context.hsv.saturation, context.hsv.value, 
                    context.rgb.red, context.rgb.green, context.rgb.blue);
    my_pwm_update_rgb(&context);
}

void my_pwm_update_rgb(my_pwm_context_t *p_context) {
    p_context->seq_values->channel_0 = p_context->rgb.red;
    p_context->seq_values->channel_1 = p_context->rgb.green;
    p_context->seq_values->channel_2 = p_context->rgb.blue;
}

void my_pwm_update_hue(my_pwm_context_t *p_context) {
    if (p_context->hsv_up) {
        p_context->hsv.hue += 1;
        if (p_context->hsv.hue >= 255) {
            p_context->hsv_up = false;
        }
    } else {
        p_context->hsv.hue -= 1;
        if (p_context->hsv.hue <= 0) {
            p_context->hsv_up = true;
        }
    }
}

void my_pwm_update_saturation(my_pwm_context_t *p_context) {
    if (p_context->hsv_up) {
        p_context->hsv.saturation += 1;
        if (p_context->hsv.saturation >= 255) {
            p_context->hsv_up = false;
        }
    } else {
        p_context->hsv.saturation -= 1;
        if (p_context->hsv.saturation <= 0) {
            p_context->hsv_up = true;
        }
    }
}

void my_pwm_update_value(my_pwm_context_t *p_context) {
    if (p_context->hsv_up) {
        p_context->hsv.value += 1;
        if (p_context->hsv.value >= 255) {
            p_context->hsv_up = false;
        }
    } else {
        p_context->hsv.value -= 1;
        if (p_context->hsv.value <= 0) {
            p_context->hsv_up = true;
        }
    }
}

void update_yellow(my_pwm_context_t *p_context) {
    if (p_context->yellow_up) {
        p_context->seq_values->channel_3 += p_context->yellow_step;
        if (p_context->seq_values->channel_3 >= 255) {
            p_context->yellow_up = false;
        }
    } else {
        p_context->seq_values->channel_3 -= p_context->yellow_step;
        if (p_context->seq_values->channel_3 <= 0) {
            p_context->yellow_up = true;
        }
    }
    
}

void my_pwm_init(void) {
    nrfx_pwm_config_t config = {
        .output_pins = {
            LED_RED   | NRFX_PWM_PIN_INVERTED,
            LED_GREEN | NRFX_PWM_PIN_INVERTED,
            LED_BLUE  | NRFX_PWM_PIN_INVERTED,
            LED_0     | NRFX_PWM_PIN_INVERTED
        },
        .irq_priority = NRFX_PWM_DEFAULT_CONFIG_IRQ_PRIORITY,
        .base_clock = NRF_PWM_CLK_125kHz,
        .count_mode = NRF_PWM_MODE_UP,
        .top_value = 255,
        .load_mode = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode = NRF_PWM_STEP_AUTO,
    };

    APP_ERROR_CHECK(nrfx_pwm_init(&pwm, &config, handler));

    context.seq_values = seq_values;
    context.yellow_step = 0;
    context.yellow_up = true;
    context.input_mode = INPUT_NO;
    context.hsv_up = true;

    nrfx_pwm_simple_playback(&pwm, &seq, 10, NRFX_PWM_FLAG_LOOP);
}