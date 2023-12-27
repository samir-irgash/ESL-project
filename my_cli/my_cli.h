#ifndef __MY_CLI
#define __MY_CLI

#include "my_pwm.h"
#include "nrf_drv_usbd.h"
#include "sdk_errors.h"
#include "app_usbd_cdc_types.h"
#include "app_usbd_cdc_acm.h"

#define READ_SIZE 1

#define CDC_ACM_COMM_INTERFACE  2
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN3

#define CDC_ACM_DATA_INTERFACE  3
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN4
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT4

ret_code_t my_cli_init(void);

void my_cli_handle_command(
    char *command,
    my_pwm_context_t *p_context
);

#endif // __MY_CLI