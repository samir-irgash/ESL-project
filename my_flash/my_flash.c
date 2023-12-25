#include <stdbool.h>
#include "nrfx_nvmc.h"
#include "my_color.h"
#include "my_flash.h"

bool my_flash_data_check(void) {
    writable_hsv data;
    uint32_t *flash_ptr = (uint32_t *) APP_DATA_STARTING_ADDRESS;

    data.writable = *flash_ptr;

    return data.writable != 0xFFFFFFFF;
}

void my_flash_write_hsv(my_color_hsv_t const *p_hsv) {
    writable_hsv data;
    data.padded.padding = 0xFF;
    data.padded.hsv = *p_hsv;

    uint32_t address = APP_DATA_STARTING_ADDRESS;

    if (!nrfx_nvmc_word_writable_check(address, data.writable)) {
        nrfx_nvmc_page_erase(address);
    }

    nrfx_nvmc_word_write(address, data.writable);

    // In case write failed
    while (!nrfx_nvmc_write_done_check()){
        nrfx_nvmc_word_write(address, data.writable);
    }
}

void my_flash_read_hsv(my_color_hsv_t *p_hsv) {
    writable_hsv data;
    uint32_t *flash_ptr = (uint32_t *) APP_DATA_STARTING_ADDRESS;

    data.writable = *flash_ptr;

    *p_hsv = data.padded.hsv;
}