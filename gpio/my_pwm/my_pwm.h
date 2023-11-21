#ifndef __MY_PWM
#define __MY_PWM

#define PWM_FREQUENCY_HZ 1000
#define PWM_PERIOD_US 1000000 / PWM_FREQUENCY_HZ
#define DUTY_CYCLE_MAX_MILLI 1000
#define DUTY_CYCLE_MIN_MILLI 0
#define DUTY_CYCLE_STEP_MILLI 2
#define BLINK_DURATION_MS 100
#define STEP_US 100

extern volatile bool pause;

int timeon_us(int duty_cycle_milli_arg);

void my_pwm_turn_on(uint32_t led_i);

void my_pwm_turn_off(uint32_t led_i);

void my_pwm_blink(uint32_t led_i);

#endif // __MY_PWM