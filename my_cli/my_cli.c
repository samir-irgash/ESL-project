#include <inttypes.h>
#include <stdlib.h>
#include "my_cli.h"
#include "app_usbd.h"
#include "app_usbd_serial_num.h"
#include "app_usbd_cdc_acm.h"
#include "sdk_errors.h"
#include "nrf_log.h"
#include "my_pwm.h"
#include "my_flash.h"
#include "my_command.h"

static char command_buffer[64];
static char feedback_buffer[64];
static int ptr = 0;
static char m_rx_buffer[READ_SIZE];

const char help_me[] = 
    "\rUsage:\r\nRGB <red> <green> <blue>:\r\n\tset rgb, all in range 0-255\r\nHSV <hue> <saturation> <value>\r\n\tset hsv, all in range 0-255\r\n";

static void usb_ev_handler(app_usbd_class_inst_t const * p_inst,
                           app_usbd_cdc_acm_user_event_t event);

APP_USBD_CDC_ACM_GLOBAL_DEF(usb_cdc_acm,
                            usb_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_NONE);

static void usb_ev_handler(app_usbd_class_inst_t const * p_inst, app_usbd_cdc_acm_user_event_t event)
{
    switch (event)
    {
    case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
    {
        ret_code_t ret;
        ret = app_usbd_cdc_acm_read(&usb_cdc_acm, m_rx_buffer, READ_SIZE);
        UNUSED_VARIABLE(ret);
        break;
    }
    case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
    {
        break;
    }
    case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
    {
        break;
    }
    case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
    {
        ret_code_t ret;
        do
        {
            size_t size = app_usbd_cdc_acm_rx_size(&usb_cdc_acm);
            UNUSED_VARIABLE(size);

            if (m_rx_buffer[0] == '\r' || m_rx_buffer[0] == '\n')
            {
                ret = app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\n", 2);

                command_buffer[ptr] = ' ';
                my_cli_handle_command(command_buffer, &context);
            }
            else
            {
                command_buffer[ptr] = m_rx_buffer[0];
                ptr += 1;

                ret = app_usbd_cdc_acm_write(&usb_cdc_acm,
                                             m_rx_buffer,
                                             READ_SIZE);
            }

            /* Fetch data until internal buffer is empty */
            ret = app_usbd_cdc_acm_read(&usb_cdc_acm,
                                        m_rx_buffer,
                                        READ_SIZE);
        } while (ret == NRF_SUCCESS);

        break;
    }
    default:
        break;
    }
}

void my_cli_handle_command(char *command, my_pwm_context_t *p_context) {
    ret_code_t ret;
    parsed_command_t parsed_command = parse_string(command);
    if (!strcmp(parsed_command.args[0], "RGB")) {
        unsigned int r, g, b;
        my_color_rgb_t rgb;
        r = atoi(parsed_command.args[1]);
        g = atoi(parsed_command.args[2]);
        b = atoi(parsed_command.args[3]);
        NRF_LOG_INFO("Params received: %u, %u, %u", r, g, b);
        rgb.red = (uint8_t) r;
        rgb.green = (uint8_t) g;
        rgb.blue = (uint8_t) b;
        my_color_rgb2hsv(&rgb, &p_context->hsv);
        my_flash_write_hsv(&p_context->hsv);

        sprintf(feedback_buffer, "RGB value is set to: R=%d, G=%d, B=%d\r\n", r, g, b);
        ret = app_usbd_cdc_acm_write(&usb_cdc_acm, feedback_buffer, strlen(feedback_buffer));
    }

    else if (command[0] == 'H') {
        unsigned int h, s, v;
        h = atoi(parsed_command.args[1]);
        s = atoi(parsed_command.args[2]);
        v = atoi(parsed_command.args[3]);

        NRF_LOG_INFO("Params received: %u, %u, %u", h, s, v);

        p_context->hsv.hue = (uint8_t) h;
        p_context->hsv.saturation = (uint8_t) s;
        p_context->hsv.value = (uint8_t) v;
        my_flash_write_hsv(&p_context->hsv);

        sprintf(feedback_buffer, "HSV value is set to: H=%d, S=%d, V=%d\r\n", h, s, v);
        ret = app_usbd_cdc_acm_write(&usb_cdc_acm, feedback_buffer, strlen(feedback_buffer));

    }

    else if (command[0] == 'h') {
        ret = app_usbd_cdc_acm_write(&usb_cdc_acm, help_me, strlen(help_me));
    }


    UNUSED_VARIABLE(ret);

    ptr = 0;
    memset(command, 0, 64);
    memset(feedback_buffer, 0, 64);
}

ret_code_t my_cli_init(void) {
    app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&usb_cdc_acm);
    ret_code_t ret = app_usbd_class_append(class_cdc_acm);
    memset(command_buffer, 0, 64);
    memset(feedback_buffer, 0, 64);

    return ret;
}