#ifndef __MY_FLASH
#define __MY_FLASH

#include "sdk_config.h"

#define BOOTLOADER_STARTING_ADDRESS 0x000E0000
#define PAGE_SIZE 0x1000
#define APP_DATA_PAGE_NUMBER 3
#define APP_DATA_STARTING_ADDRESS BOOTLOADER_STARTING_ADDRESS - APP_DATA_PAGE_NUMBER * PAGE_SIZE

typedef struct {
    my_color_hsv_t hsv;
    uint8_t padding;
} hsv_padded_t;

typedef union {
    hsv_padded_t padded;
    uint32_t writable;
} writable_hsv;

bool my_flash_data_check(void);

void my_flash_write_hsv(
    my_color_hsv_t const *p_hsv
);

void my_flash_read_hsv(
    my_color_hsv_t *p_hsv
);

void my_flash_init(void);
void wait_for_flash(void);

#endif // __MY_FLASH