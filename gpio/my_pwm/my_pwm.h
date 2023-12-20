#ifndef __MY_PWM
#define __MY_PWM

#define BLINK_DURATION_MS 1000
#define PWM_FREQUENCY_HZ 1000
#define PWM_PERIOD_US 1000000 / PWM_FREQUENCY_HZ
#define DUTY_CYCLE_MAX_MILLI 1000
#define DUTY_CYCLE_MIN_MILLI 0
#define DUTY_CYCLE_STEP_MILLI PWM_PERIOD_US / BLINK_DURATION_MS

typedef enum {
    BLINK_BEGIN,
    BLINK_END,
} my_pwm_state_e;

typedef struct {
    uint32_t led_i;                             // Led Index, indicates which led is currently blinking
    uint32_t duty_cycle_milli;                  // Duty cycle *10^-3
    uint32_t duty_cycle_step_milli;             // Duty cycle increment/decrement step (indicated by blink duration)
    uint32_t next_exec_time_us;                 // Used in order not to block with while loop
    nrfx_systick_state_t pwm_systick_state;     // Systick state from last blink context, used to compare with next_exec_time_us
    bool pwm_on;                                // pwm_on = true indicates LED should be set, pwm_off = true indicates LED should be reset
    bool take_step;                             // Boolean to indicate if step should be taken
    my_pwm_state_e pwm_state;                   // state to track the ending of blink
} blink_context_t;

extern volatile bool pause;

int time_us(int duty_cycle_milli_arg);

bool my_pwm_blink(blink_context_t *context);

void blink_n_times(blink_context_t *context, int n);

void my_pwm_step(blink_context_t *context;);

#endif // __MY_PWM