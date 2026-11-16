// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>
#include <stddef.h>

// The display form "#RRGGBB" has 7 characters and a null terminator.
#define COLOR_STR_SIZE 8

typedef struct {
    unsigned char r, g, b;
} Color;

// color_make sets *out and returns true when each value is 0 to 255.
bool color_make(int r, int g, int b, Color *out);

// color_format writes the display form of c into buf and returns buf.
char *color_format(Color c, char buf[COLOR_STR_SIZE]);

// color_debug writes the debug form "Color(r=255, g=128, b=0)" into buf, which has size bytes.
// It returns the length of the full text, as snprintf does; a value of size or more means the text was cut.
int color_debug(Color c, char *buf, size_t size);

// format_colors writes the display form of each color into out[i]. It returns 0, or -1 when a value is out of range.
int format_colors(const int *reds, const int *greens, const int *blues, size_t n, char (*out)[COLOR_STR_SIZE]);

#endif
