// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

#include "split.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

// check compares the fields with want, an array of strings that ends with NULL.
static void check(const char *name, struct field *got, size_t count, const char *const *want) {
    size_t i = 0;
    int ok = got != NULL;
    for (; ok && want[i] != NULL; i++) {
        ok = i < count && got[i].len == strlen(want[i]) &&
             memcmp(got[i].start, want[i], got[i].len) == 0;
    }
    if (!ok || i != count) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(got);
}

// Each macro calls the split function before check, because C does not fix the order in which the arguments of a call are evaluated, and check reads n.
#define WANT(...) ((const char *const[]){__VA_ARGS__, NULL})
#define NONE ((const char *const[]){NULL})

#define SPLIT(name, s, sep, limit, want)              \
    do {                                              \
        size_t n = 0;                                 \
        struct field *got = split(s, sep, limit, &n); \
        check(name, got, n, want);                    \
    } while (0)

#define SPLIT_WS(name, s, want)                      \
    do {                                             \
        size_t n = 0;                                \
        struct field *got = split_whitespace(s, &n); \
        check(name, got, n, want);                   \
    } while (0)

#define SPLIT_RE(name, s, pattern, want)                 \
    do {                                                 \
        size_t n = 0;                                    \
        struct field *got = split_regex(s, pattern, &n); \
        check(name, got, n, want);                       \
    } while (0)

int main(void) {
    SPLIT("comma", "a,b,c", ",", 0, WANT("a", "b", "c"));
    SPLIT("csv-line", "Ada,1815,London", ",", 0, WANT("Ada", "1815", "London"));
    SPLIT("multi-char-separator", "a::b::c", "::", 0, WANT("a", "b", "c"));
    SPLIT("space-separator", "one two  three", " ", 0, WANT("one", "two", "", "three"));
    SPLIT("limit-two", "key=value=more", "=", 2, WANT("key", "value=more"));
    SPLIT("empty", "", ",", 0, WANT(""));
    SPLIT("no-separator", "abc", ",", 0, WANT("abc"));
    SPLIT("leading-separator", ",a,b", ",", 0, WANT("", "a", "b"));
    SPLIT("trailing-separators", "a,b,,", ",", 0, WANT("a", "b", "", ""));
    SPLIT("adjacent-separators", "a,,b", ",", 0, WANT("a", "", "b"));
    SPLIT("only-separator", ",", ",", 0, WANT("", ""));
    SPLIT("overlapping-match", "aaa", "aa", 0, WANT("", "a"));
    SPLIT("dot-separator", "192.168.0.1", ".", 0, WANT("192", "168", "0", "1"));
    SPLIT("pipe-separator", "a|b|c", "|", 0, WANT("a", "b", "c"));
    SPLIT("non-ascii", "día→naïve→日本→🙂", "→", 0, WANT("día", "naïve", "日本", "🙂"));
    SPLIT("limit-one", "a,b,c", ",", 1, WANT("a,b,c"));
    SPLIT("limit-above-count", "a,b", ",", 5, WANT("a", "b"));
    SPLIT("limit-keeps-trailing-empty", "a,b,", ",", 3, WANT("a", "b", ""));

    SPLIT_WS("single-spaces", "a b c", WANT("a", "b", "c"));
    SPLIT_WS("runs-and-ends", "  lead  and trail  ", WANT("lead", "and", "trail"));
    SPLIT_WS("tabs-and-newlines", "a\t\tb\nc\n", WANT("a", "b", "c"));
    SPLIT_WS("one-word", "word", WANT("word"));
    SPLIT_WS("empty", "", NONE);
    SPLIT_WS("only-white-space", " \t\n ", NONE);

    SPLIT_RE("comma-or-semicolon", "a, b;c ,d", "[ ]*[,;][ ]*", WANT("a", "b", "c", "d"));
    SPLIT_RE("digit-runs", "a1b22c333", "[0-9]+", WANT("a", "b", "c", ""));
    SPLIT_RE("adjacent-matches", "a,;b", "[,;]", WANT("a", "", "b"));
    SPLIT_RE("leading-and-trailing", ";a;", ";", WANT("", "a", ""));
    SPLIT_RE("empty", "", "[,;]", WANT(""));
    SPLIT_RE("no-match", "abc", "[0-9]+", WANT("abc"));

    size_t n;
    if (split_regex("a", "[a", &n) != NULL) {
        fprintf(stderr, "bad pattern: no error\n");
        failures++;
    }
    return failures == 0 ? 0 : 1;
}
