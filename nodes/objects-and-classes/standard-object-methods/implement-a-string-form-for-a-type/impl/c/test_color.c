// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

#include "color.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const int *reds, const int *greens, const int *blues, size_t n,
                  const char *const *want) {
    char out[4][COLOR_STR_SIZE];
    if (format_colors(reds, greens, blues, n, out) != 0) {
        fprintf(stderr, "case %s failed: error\n", name);
        failures++;
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (strcmp(out[i], want[i]) != 0) {
            fprintf(stderr, "case %s failed at %zu: got %s, want %s\n", name, i, out[i], want[i]);
            failures++;
            return;
        }
    }
}

static void expect(const char *name, bool ok) {
    if (!ok) {
        fprintf(stderr, "check %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("orange", (int[]){255}, (int[]){128}, (int[]){0}, 1, (const char *[]){"#FF8000"});
    check("three", (int[]){255, 0, 0}, (int[]){0, 255, 0}, (int[]){0, 0, 255}, 3,
          (const char *[]){"#FF0000", "#00FF00", "#0000FF"});
    check("mixed", (int[]){18, 171}, (int[]){52, 205}, (int[]){86, 239}, 2, (const char *[]){"#123456", "#ABCDEF"});
    check("empty", NULL, NULL, NULL, 0, NULL);
    check("single", (int[]){1}, (int[]){2}, (int[]){3}, 1, (const char *[]){"#010203"});
    check("zero", (int[]){0}, (int[]){0}, (int[]){0}, 1, (const char *[]){"#000000"});
    check("max", (int[]){255}, (int[]){255}, (int[]){255}, 1, (const char *[]){"#FFFFFF"});
    check("padding", (int[]){10, 0}, (int[]){11, 15}, (int[]){12, 16}, 2, (const char *[]){"#0A0B0C", "#000F10"});

    Color c;
    expect("make", color_make(255, 128, 0, &c));
    expect("bad value", !color_make(256, 0, 0, &c));
    char buf[COLOR_STR_SIZE];
    expect("display", strcmp(color_format(c, buf), "#FF8000") == 0);
    char debug[64];
    expect("debug", color_debug(c, debug, sizeof debug) == 24 && strcmp(debug, "Color(r=255, g=128, b=0)") == 0);
    char small[10];
    expect("debug cut", color_debug(c, small, sizeof small) == 24 && strcmp(small, "Color(r=2") == 0);

    if (failures == 0) {
        printf("all tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}
