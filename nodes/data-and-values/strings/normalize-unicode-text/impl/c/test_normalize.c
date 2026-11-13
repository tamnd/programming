// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

#include "normalize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *s, const char *form, const char *want) {
    char *got = normalize(s, form);
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got %s, want %s\n", name, got == NULL ? "NULL" : got, want);
        failures++;
    }
    free(got);
}

// U+00E9 is NFC, and e U+0301 is NFD. Both are the same text.
int main(void) {
    check("nfc-composes", "e\xcc\x81", "NFC", "\xc3\xa9");
    check("nfd-decomposes", "\xc3\xa9", "NFD", "e\xcc\x81");
    check("ascii", "abc", "NFC", "abc");
    check("empty", "", "NFD", "");
    check("already-nfc", "\xc3\xa9", "NFC", "\xc3\xa9");
    check("already-nfd", "e\xcc\x81", "NFD", "e\xcc\x81");
    check("mixed-nfc", "cafe\xcc\x81 caf\xc3\xa9", "NFC", "caf\xc3\xa9 caf\xc3\xa9");
    check("mixed-nfd", "cafe\xcc\x81 caf\xc3\xa9", "NFD", "cafe\xcc\x81 cafe\xcc\x81");
    check("upper-grave", "E\xcc\x80", "NFC", "\xc3\x88");
    check("diaeresis", "u\xcc\x88" "ber", "NFC", "\xc3\xbc" "ber");
    check("circumflex", "\xc3\xae", "NFD", "i\xcc\x82");
    check("umlauts", "\xc3\x84\xc3\x96\xc3\x9c\xc3\xa4\xc3\xb6\xc3\xbc", "NFD", "A\xcc\x88O\xcc\x88U\xcc\x88" "a\xcc\x88o\xcc\x88u\xcc\x88");
    check("spacing-accent", "\xc2\xb4" "e", "NFC", "\xc2\xb4" "e");
    check("no-break-space", "a\xc2\xa0" "b", "NFD", "a\xc2\xa0" "b");
    check("sharp-s", "stra\xc3\x9f" "e", "NFD", "stra\xc3\x9f" "e");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
