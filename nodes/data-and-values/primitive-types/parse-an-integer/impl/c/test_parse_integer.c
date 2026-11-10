// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

#include "parse_integer.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void ok(const char *name, const char *s, int base, int64_t want) {
    int64_t got = 0;
    enum parse_status status = parse_integer(s, base, &got);
    if (status != PARSE_OK || got != want) {
        fprintf(stderr, "case %s failed: status %d, got %" PRId64 "\n", name, (int)status, got);
        failures++;
    }
}

static void fails(const char *name, const char *s, int base, enum parse_status want) {
    int64_t got = 0;
    enum parse_status status = parse_integer(s, base, &got);
    if (status != want) {
        fprintf(stderr, "case %s failed: status %d, want %d\n", name, (int)status, (int)want);
        failures++;
    }
}

// repeat returns n copies of c followed by tail, in memory from malloc.
static char *repeat(char c, size_t n, const char *tail) {
    size_t len = strlen(tail);
    char *s = malloc(n + len + 1);
    if (s == NULL) {
        exit(1);
    }
    memset(s, c, n);
    memcpy(s + n, tail, len + 1);
    return s;
}

int main(void) {
    char *zeros = repeat('0', 5000, "42");
    char *nines = repeat('9', 5000, "");

    ok("decimal", "42", 10, INT64_C(42));
    ok("negative", "-17", 10, INT64_C(-17));
    ok("hex", "ff", 16, INT64_C(255));
    ok("binary", "101010", 2, INT64_C(42));
    ok("base-36", "zz", 36, INT64_C(1295));
    ok("plus-sign", "+5", 10, INT64_C(5));
    ok("negative-zero", "-0", 10, INT64_C(0));
    ok("zero", "0", 10, INT64_C(0));
    ok("leading-zeros", "007", 10, INT64_C(7));
    ok("many-leading-zeros", zeros, 10, INT64_C(42));
    ok("max", "9223372036854775807", 10, INT64_C(9223372036854775807));
    ok("min", "-9223372036854775808", 10, INT64_MIN);
    ok("max-hex", "7fffffffffffffff", 16, INT64_C(9223372036854775807));
    ok("min-binary", "-1000000000000000000000000000000000000000000000000000000000000000", 2, INT64_MIN);
    ok("upper-case", "FF", 16, INT64_C(255));
    ok("mixed-case", "Zz", 36, INT64_C(1295));
    ok("above-safe-integer", "9007199254740993", 10, INT64_C(9007199254740993));

    fails("empty", "", 10, PARSE_INVALID);
    fails("sign-only", "-", 10, PARSE_INVALID);
    fails("plus-only", "+", 10, PARSE_INVALID);
    fails("double-sign", "+-5", 10, PARSE_INVALID);
    fails("leading-space", " 42", 10, PARSE_INVALID);
    fails("trailing-space", "42 ", 10, PARSE_INVALID);
    fails("trailing-newline", "42\n", 10, PARSE_INVALID);
    fails("trailing-garbage", "42px", 10, PARSE_INVALID);
    fails("underscore", "1_000", 10, PARSE_INVALID);
    fails("thousands-separator", "1,000", 10, PARSE_INVALID);
    fails("decimal-point", "4.0", 10, PARSE_INVALID);
    fails("exponent", "1e3", 10, PARSE_INVALID);
    fails("hex-prefix", "0x1A", 16, PARSE_INVALID);
    fails("binary-prefix", "0b101", 2, PARSE_INVALID);
    fails("digit-outside-base", "102", 2, PARSE_INVALID);
    fails("letter-outside-base", "fg", 16, PARSE_INVALID);
    fails("arabic-indic-digits", "\xd9\xa1\xd9\xa2", 10, PARSE_INVALID);
    fails("fullwidth-digits", "\xef\xbc\x91\xef\xbc\x92", 10, PARSE_INVALID);
    fails("kelvin-sign", "\xe2\x84\xaa", 36, PARSE_INVALID);
    fails("max-plus-one", "9223372036854775808", 10, PARSE_RANGE);
    fails("min-minus-one", "-9223372036854775809", 10, PARSE_RANGE);
    fails("two-to-the-64", "18446744073709551616", 10, PARSE_RANGE);
    fails("hex-sign-bit", "8000000000000000", 16, PARSE_RANGE);
    fails("hex-all-ones", "ffffffffffffffff", 16, PARSE_RANGE);
    fails("min-times-ten", "-92233720368547758080", 10, PARSE_RANGE);
    fails("many-digits", nines, 10, PARSE_RANGE);

    free(zeros);
    free(nines);
    if (failures > 0) {
        fprintf(stderr, "%d cases failed\n", failures);
        return 1;
    }
    printf("all cases passed\n");
    return 0;
}
