// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

#include "color.h"

#include <stdio.h>

bool color_make(int r, int g, int b, Color *out) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        return false;
    }
    *out = (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b};
    return true;
}

char *color_format(Color c, char buf[COLOR_STR_SIZE]) {
    snprintf(buf, COLOR_STR_SIZE, "#%02X%02X%02X", c.r, c.g, c.b);
    return buf;
}

int color_debug(Color c, char *buf, size_t size) {
    return snprintf(buf, size, "Color(r=%d, g=%d, b=%d)", c.r, c.g, c.b);
}

int format_colors(const int *reds, const int *greens, const int *blues, size_t n, char (*out)[COLOR_STR_SIZE]) {
    for (size_t i = 0; i < n; i++) {
        Color c;
        if (!color_make(reds[i], greens[i], blues[i], &c)) {
            return -1;
        }
        color_format(c, out[i]);
    }
    return 0;
}
