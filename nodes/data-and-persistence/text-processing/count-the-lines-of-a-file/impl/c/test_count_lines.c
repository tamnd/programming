// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

#include "count_lines.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "count_lines_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check(const char *name, const char *data, size_t n, uintmax_t want_newlines, uintmax_t want_lines) {
    write_file(data, n);
    uintmax_t newlines = 99, lines = 99;
    if (count_lines(path, &newlines, &lines) != 0 || newlines != want_newlines || lines != want_lines) {
        fprintf(stderr, "case %s: got %ju, %ju\n", name, newlines, lines);
        exit(1);
    }
}

// sizeof counts the bytes of the literal, also a NUL byte inside it, but not the final NUL.
#define CASE(name, data, newlines, lines) check(name, data, sizeof(data) - 1, newlines, lines)

int main(void) {
    CASE("lf", "alpha\nbeta\ngamma\n", 3, 3);
    CASE("blank-lines", "first\n\n\nlast\n", 4, 4);
    CASE("empty", "", 0, 0);
    CASE("only-newline", "\n", 1, 1);
    CASE("no-final-newline", "one\ntwo\nthree", 2, 3);
    CASE("one-line-no-newline", "x", 0, 1);
    CASE("crlf", "one\r\ntwo\r\n", 2, 2);
    CASE("crlf-no-final-newline", "one\r\ntwo", 1, 2);
    CASE("cr-only", "one\rtwo\r", 0, 1);
    CASE("other-breaks", "a\vb\fc\xc2\x85" "d\xe2\x80\xa8" "e\n", 1, 1);
    CASE("invalid-utf8", "ok\n\xff\xfe\n", 2, 2);
    CASE("nul-bytes", "a\0b\n\0\n", 2, 2);

    // 100000 lines and a last line with no LF: more than one buffer of 64 KiB.
    size_t n = 2 * 100000 + 3;
    char *big = malloc(n);
    assert(big != NULL);
    for (size_t i = 0; i < 100000; i++) {
        big[2 * i] = 'x';
        big[2 * i + 1] = '\n';
    }
    memcpy(big + 2 * 100000, "end", 3);
    check("many-chunks", big, n, 100000, 100001);
    free(big);
    remove(path);

    uintmax_t newlines, lines;
    errno = 0;
    if (count_lines("count_lines_missing.txt", &newlines, &lines) != -1 || errno != ENOENT) {
        fprintf(stderr, "case missing: no ENOENT error\n");
        return 1;
    }
    puts("ok");
    return 0;
}
