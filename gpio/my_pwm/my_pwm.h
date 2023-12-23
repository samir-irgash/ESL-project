#ifndef __MY_PWM
#define __MY_PWM

#include "nrfx_pwm.h"
#include "nrf_pwm.h"
#include "my_color.h"

typedef enum {
    INPUT_NO,
    INPUT_HUE,
    INPUT_SATURATION,
    INPUT_VALUE
} my_pwm_input_mode;

typedef struct {
    nrf_pwm_values_individual_t *seq_values;
    uint32_t yellow_step;
    bool yellow_up;
    my_pwm_input_mode input_mode;
    my_color_hsv_t hsv;
    my_color_rgb_t rgb;
    bool hsv_up;
} my_pwm_context_t;

extern my_pwm_context_t context;

void my_pwm_init(void);

my_pwm_input_mode my_pwm_next_mode(
  my_pwm_input_mode mode
);

void update_yellow(
  my_pwm_context_t *p_context
);

void my_pwm_handle_hold(void);

void my_pwm_update_hue(
  my_pwm_context_t *p_context
);

void my_pwm_update_saturation(
  my_pwm_context_t *p_context
);

void my_pwm_update_value(
  my_pwm_context_t *p_context
);

void my_pwm_update_rgb(
  my_pwm_context_t *p_context
);

#endif // __MY_PWM