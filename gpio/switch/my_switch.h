#ifndef __MY_SWITCH
#define __MY_SWITCH

#include "nrfx_gpiote.h"
#include "nrf_gpiote.h"

/*
* @macro CLICK_DURATION_MS 
* The time for which controller will wait for switch to be released
* to be counted as click.
* 200 - too much, fast double click is ignored
* 100 - too fast, slow double click is ignored
*/
#define CLICK_DURATION_MS 150
#define DOUBLECLICK_ALLOWED_INTERVAL_MS 500
#define BOUNCE_DURATION_MS 15

typedef enum {
    SWITCH_RELEASED,
    SWITCH_PRESSED
} my_switch_state_e;

extern volatile my_switch_state_e switch_state;
extern volatile int click_cnt;
extern volatile bool pause;

void my_switch_debounce_handler(void *p_context);

void my_switch_click_handler(void *p_context);

void SW1_IRQHandler(nrfx_gpiote_pin_t pin, 
                    nrf_gpiote_polarity_t action);

ret_code_t my_switch_init(void);


#endif // MY_SWITCH
