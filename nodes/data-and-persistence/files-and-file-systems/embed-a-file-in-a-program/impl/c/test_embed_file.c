/* SPDX-License-Identifier: MIT */
/* Part of https://programming.tamnd.com/embed-a-file-in-a-program/. */

#include "embed_file.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, long n, const char *want) {
    size_t len = 0;
    const char *got = embedded_line(n, &len);
    int ok = want == NULL ? got == NULL
                          : got != NULL && len == strlen(want) && memcmp(got, want, len) == 0;
    if (!ok) {
        printf("FAIL %s: line %ld\n", name, n);
        failures++;
    }
}

/* The fallback header must hold the same bytes as lines.txt. */
static const unsigned char header[] = {
#include "lines_txt.h"
};

int main(void) {
    check("first-line", 1, "alpha");
    check("second-line", 2, "caf\xc3\xa9");
    check("last-line", 4, "gamma");
    check("non-ascii-line", 2, "caf\xc3\xa9");
    check("empty-line", 3, "");
    check("zero", 0, NULL);
    check("negative", -1, NULL);
    check("past-the-end", 5, NULL);

    size_t len = 0;
    const unsigned char *data = embedded_data(&len);
    const char *want = "# SPDX-License-Identifier: MIT\nalpha\ncaf\xc3\xa9\n\ngamma\n";
    if (len != 50 || memcmp(data, want, len) != 0) {
        printf("FAIL bytes: %zu bytes\n", len);
        failures++;
    }
    if (sizeof header != len || memcmp(header, data, len) != 0) {
        printf("FAIL header: lines_txt.h does not match lines.txt\n");
        failures++;
    }
    if (failures == 0) {
        printf("ok\n");
    }
    return failures == 0 ? 0 : 1;
}
