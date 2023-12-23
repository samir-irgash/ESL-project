#include <stdint.h>
#include "nrfx_pwm.h"
#include "my_color.h"

/*
* Props to https://stackoverflow.com/a/14733008
*/

void my_color_hsv2rgb(my_color_hsv_t const *in, my_color_rgb_t *out) {
    uint8_t region, remainder, p, q, t;

    if (in->saturation == 0) {
        out->red = in->value;
        out->green = in->value;
        out->blue = in->value;
        return;
    }

    region = in->hue / 43;
    remainder = (in->hue - (region * 43)) * 6;

    p = (in->value * (255 - in->saturation)) >> 8;
    q = (in->value * (255 - ((in->saturation * remainder) >> 8))) >> 8;
    t = (in->value * (255 - ((in->saturation * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            out->red = in->value;
            out->green = t;
            out->blue = p;
            break;
        case 1:
            out->red = q;
            out->green = in->value;
            out->blue = p;
            break;
        case 2:
            out->red = p;
            out->green = in->value;
            out->blue = t;
            break;
        case 3:
            out->red = p;
            out->green = q;
            out->blue = in->value;
            break;
        case 4:
            out->red = t;
            out->green = p;
            out->blue = in->value;
            break;
        default:
            out->red = in->value;
            out->green = p;
            out->blue = q;
            break;
    }
}

void my_color_rgb2hsv(my_color_rgb_t const *in, my_color_hsv_t *out) {
    uint8_t rgb_min, rgb_max;

    rgb_min = in->red < in->green ? (in->red < in->blue ? in->red : in->blue) : (in->green < in->blue ? in->green : in->blue);
    rgb_max = in->red > in->green ? (in->red > in->blue ? in->red : in->blue) : (in->green > in->blue ? in->green : in->blue);

    out->value = rgb_max;
    if (out->value == 0) {
        out->hue = 0;
        out->saturation = 0;
        return;
    }

    out->saturation = 255 * (long)(rgb_max - rgb_min) / out->value;
    if (out->saturation == 0) {
        out->hue = 0;
        return;
    }

    if (rgb_max == in->red) {
        out->hue = 0 + 43 * (in->green - in->blue) / (rgb_max - rgb_min);
    } else if (rgb_max == in->green) {
        out->hue = 85 + 43 * (in->blue - in->red) / (rgb_max - rgb_min);
    } else {
        out->hue = 171 + 43 * (in->red - in->green) / (rgb_max - rgb_min);
    }
}
