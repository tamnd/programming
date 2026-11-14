// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

#include "word_count.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "word_count_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check(const char *name, const char *data, size_t n, uintmax_t lines, uintmax_t words, uintmax_t bytes) {
    write_file(data, n);
    struct word_count c = {99, 99, 99};
    if (word_count(path, &c) != 0 || c.lines != lines || c.words != words || c.bytes != bytes) {
        fprintf(stderr, "case %s: got %ju %ju %ju\n", name, c.lines, c.words, c.bytes);
        exit(1);
    }
}

static void check_invalid(const char *name, const char *data, size_t n) {
    write_file(data, n);
    struct word_count c;
    errno = 0;
    if (word_count(path, &c) != -1 || errno != EILSEQ) {
        fprintf(stderr, "case %s: no EILSEQ error\n", name);
        exit(1);
    }
}

// sizeof counts the bytes of the literal, but not the final NUL. A hex escape
// takes all the hex digits after it, so a literal ends after each escape that
// a letter from a to f follows.
#define CASE(name, data, lines, words, bytes) check(name, data, sizeof(data) - 1, lines, words, bytes)
#define INVALID(name, data) check_invalid(name, data, sizeof(data) - 1)

int main(void) {
    CASE("simple", "one two three\n", 1, 3, 14);
    CASE("multiple-spaces", "  a   b  \n", 1, 2, 10);
    CASE("tabs-and-lines", "a\tb\nc\n", 2, 3, 6);
    CASE("empty", "", 0, 0, 0);
    CASE("only-white-space", " \n\t\n", 2, 0, 4);
    CASE("no-final-newline", "one two", 0, 2, 7);
    CASE("crlf", "a b\r\nc\r\n", 2, 3, 8);
    CASE("non-ascii", "na\xc3\xaf" "ve caf\xc3\xa9\n", 1, 2, 13);
    CASE("unicode-spaces", "a\xc2\xa0" "b\xe3\x80\x80" "c\xe2\x80\xa8" "d\n", 1, 4, 13);
    CASE("not-white-space", "a\xe2\x80\x8b" "b\xc2\xad" "c\xef\xbb\xbf" "d\n", 1, 1, 13);
    CASE("control-separators", "a\x1c" "b\x1f" "c\n", 1, 1, 6);
    CASE("nel", "a\xc2\x85" "b\n", 1, 2, 5);

    // 100000 lines: more than one buffer of 64 KiB.
    size_t n = 4 * 100000;
    char *big = malloc(n);
    assert(big != NULL);
    for (size_t i = 0; i < 100000; i++) {
        memcpy(big + 4 * i, "a b\n", 4);
    }
    check("many-lines", big, n, 100000, 200000, 400000);
    free(big);

    // A long line, and U+3000 at the end of the first 64 KiB.
    n = 64 * 1024 + 4;
    big = malloc(n);
    assert(big != NULL);
    memset(big, 'x', 64 * 1024 - 1);
    memcpy(big + 64 * 1024 - 1, "\xe3\x80\x80y\n", 5);
    check("chunk-boundary", big, n, 1, 2, n);
    free(big);

    INVALID("invalid-bytes", "ok\xff\xfe\n");
    INVALID("truncated", "ok \xe2\x82");
    INVALID("surrogate", "a\xed\xa0\x80" "b\n");
    remove(path);

    struct word_count c;
    errno = 0;
    if (word_count("word_count_missing.txt", &c) != -1 || errno != ENOENT) {
        fprintf(stderr, "case missing: no ENOENT error\n");
        return 1;
    }
    puts("ok");
    return 0;
}
