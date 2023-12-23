#ifndef __MY_COLOR
#define __MY_COLOR

#include <stdint.h>

#define HUE_CONVERT(deg) (deg * 255 / 360)

typedef struct {
    uint8_t red;            // 0 to 255
    uint8_t green;          // 0 to 255
    uint8_t blue;           // 0 to 255
} my_color_rgb_t;

typedef struct {
    uint8_t hue;            // 0 to 100
    uint8_t saturation;     // 0 to 100
    uint8_t value;          // 0 to 100
} my_color_hsv_t;

void my_color_hsv2rgb(
    my_color_hsv_t const *in,
    my_color_rgb_t *out
);

void my_color_rgb2hsv(
    my_color_rgb_t const *in,
    my_color_hsv_t *out
);

#endif // __MY_COLOR
