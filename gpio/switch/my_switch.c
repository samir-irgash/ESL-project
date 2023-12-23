#include "../my_gpio.h"
#include "app_timer.h"
#include "sdk_errors.h"
#include "nrfx_gpiote.h"
#include "nrfx_errors.h"
#include "my_switch.h"
#include "my_gpio.h"
#include "my_pwm.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"
/*
* @note The polarity for each action (pull-up resistor considered)
*   BUTTON ACTION   POLARITY ACTION     ENUM
*   PRESSED         HIGH TO LOW         NRF_GPIOTE_POLARITY_HITOLO
*   RELEASED        LOW TO HIGH         NRF_GPIOTE_POLARITY_LOTOHI
*/

volatile my_switch_state_e switch_state;
volatile int click_cnt;
volatile bool pause;

APP_TIMER_DEF(switch_debounce_timer);
APP_TIMER_DEF(switch_click_timer);
APP_TIMER_DEF(switch_dclick_timer);

void my_switch_debounce_handler(void *p_context) {
    if (switch_state == SWITCH_RELEASED) {
        switch_state = SWITCH_PRESSED;
        // Time allowed for 1 click
        app_timer_start(switch_click_timer, APP_TIMER_TICKS(CLICK_DURATION_MS), NULL);
    } else if (switch_state == SWITCH_PRESSED) {
        switch_state = SWITCH_RELEASED;
    }
}

void my_switch_click_handler(void *p_context) {
    if (switch_state == SWITCH_RELEASED) { // Click happened
        app_timer_start(switch_dclick_timer, APP_TIMER_TICKS(DOUBLECLICK_ALLOWED_INTERVAL_MS), NULL);
    } else { // Button on hold
        ;
    }
}

void my_switch_dclick_handler(void *p_context) {
    NRF_LOG_INFO("Double Click Happened :(");
    context.seq_values->channel_3 = 0;
    context.yellow_up = true;
    context.input_mode = my_pwm_next_mode(context.input_mode);
    switch (context.input_mode) {
        case INPUT_NO:
            context.yellow_step = 0;
            break;
        case INPUT_HUE:
            context.yellow_step = 5;
            break;
        case INPUT_SATURATION:
            context.yellow_step = 20;
            break;
        case INPUT_VALUE:
            context.seq_values->channel_3 = 255;
            context.yellow_step = 0;
            break;
    }
}

void SW1_IRQHandler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    // Debouincing
    app_timer_stop(switch_debounce_timer);
    app_timer_start(switch_debounce_timer, APP_TIMER_TICKS(BOUNCE_DURATION_MS), NULL);
}

ret_code_t my_switch_init(void) {
    if (!nrfx_gpiote_is_init()) {
        return NRFX_ERROR_INTERNAL;
    }

    nrfx_gpiote_in_config_t btn_event_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    btn_event_config.pull = NRF_GPIO_SW1_PULL;

    nrfx_err_t code_btn = nrfx_gpiote_in_init(SW1_PIN, &btn_event_config, SW1_IRQHandler);
    NRFX_ASSERT(code_btn == NRFX_SUCCESS);

    nrfx_gpiote_in_event_enable(SW1_PIN, true);

    ret_code_t code_debounce = app_timer_create(&switch_debounce_timer, APP_TIMER_MODE_SINGLE_SHOT, my_switch_debounce_handler);
    APP_ERROR_CHECK(code_debounce);

    ret_code_t code_click = app_timer_create(&switch_click_timer, APP_TIMER_MODE_SINGLE_SHOT, my_switch_click_handler);
    APP_ERROR_CHECK(code_click);

    ret_code_t code_dclick = app_timer_create(&switch_dclick_timer, APP_TIMER_MODE_SINGLE_SHOT, my_switch_dclick_handler);
    APP_ERROR_CHECK(code_dclick);

    return NRF_SUCCESS;
}

