#ifndef __MY_FLASH
#define __MY_FLASH

#define APP_DATA_STARTING_ADDRESS 0x0007F000

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

#endif // __MY_FLASH