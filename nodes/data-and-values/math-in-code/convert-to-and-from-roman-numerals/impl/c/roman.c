// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

#include "roman.h"

#include <string.h>

static const struct {
    int value;
    const char *symbol;
} table[] = {
    {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
    {100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
    {10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"}, {1, "I"},
};

int to_roman(int n, char *out, size_t size) {
    if (n < 1 || n > 3999 || size < ROMAN_SIZE) {
        return -1;
    }
    size_t len = 0;
    for (size_t i = 0; i < sizeof table / sizeof table[0]; i++) {
        while (n >= table[i].value) {
            size_t k = strlen(table[i].symbol);
            memcpy(out + len, table[i].symbol, k);
            len += k;
            n -= table[i].value;
        }
    }
    out[len] = '\0';
    return 0;
}

static int value(char c) {
    switch (c) {
    case 'I': return 1;
    case 'V': return 5;
    case 'X': return 10;
    case 'L': return 50;
    case 'C': return 100;
    case 'D': return 500;
    case 'M': return 1000;
    default: return 0;
    }
}

int from_roman(const char *s) {
    size_t len = strlen(s);
    if (len == 0 || len >= ROMAN_SIZE) {
        return -1;
    }
    int total = 0;
    for (size_t i = 0; i < len; i++) {
        int v = value(s[i]);
        if (v == 0) {
            return -1;
        }
        int next = i + 1 < len ? value(s[i + 1]) : 0;
        total += v < next ? -v : v;
    }
    char back[ROMAN_SIZE];
    if (to_roman(total, back, sizeof back) != 0 || strcmp(back, s) != 0) {
        return -1;
    }
    return total;
}
