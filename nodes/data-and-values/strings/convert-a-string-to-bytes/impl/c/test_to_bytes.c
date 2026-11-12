// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

#include "to_bytes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

// check compares the n bytes of got with want, and then frees got.
static void check(const char *name, const char *s, const unsigned char *want, size_t n) {
    size_t k = 0;
    unsigned char *got = to_bytes(s, &k);
    if (got == NULL || k != n || (n > 0 && memcmp(got, want, n) != 0)) {
        fprintf(stderr, "case %s: got %zu bytes, want %zu\n", name, k, n);
        failures++;
    }
    free(got);
}

// The encoding is UTF-8. U+00E9 is 2 bytes, U+20AC is 3 bytes and U+1F642 is 4 bytes. In Latin-1, U+00FF would be 1 byte.
int main(void) {
    check("ascii", "abc", (const unsigned char[]){97, 98, 99}, 3);
    check("letter", "A", (const unsigned char[]){65}, 1);
    check("punct", "hi!", (const unsigned char[]){104, 105, 33}, 3);
    check("empty", "", NULL, 0);
    check("control", "\t\n", (const unsigned char[]){9, 10}, 2);
    check("two-byte", "\xc3\xa9", (const unsigned char[]){195, 169}, 2);
    check("latin-1-max", "\xc3\xbf", (const unsigned char[]){195, 191}, 2);
    check("three-byte", "\xe2\x82\xac", (const unsigned char[]){226, 130, 172}, 3);
    check("bom", "\xef\xbb\xbf", (const unsigned char[]){239, 187, 191}, 3);
    check("astral", "\xf0\x9f\x99\x82", (const unsigned char[]){240, 159, 153, 130}, 4);
    check("combining", "e\xcc\x81", (const unsigned char[]){101, 204, 129}, 3);
    check("mixed", "a\xc3\xa9\xf0\x9f\x99\x82", (const unsigned char[]){97, 195, 169, 240, 159, 153, 130}, 7);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
