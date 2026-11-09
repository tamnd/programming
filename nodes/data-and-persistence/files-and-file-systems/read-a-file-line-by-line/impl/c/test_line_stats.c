// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

#include "line_stats.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "line_stats_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

#define CASE(name, data, want_lines, want_longest) \
    check(name, data, sizeof(data) - 1, want_lines, want_longest)

static void check(const char *name, const char *data, size_t n,
                  size_t want_lines, size_t want_longest) {
    write_file(data, n);
    size_t lines = 99, longest = 99;
    if (line_stats(path, &lines, &longest) != 0 || lines != want_lines ||
        longest != want_longest) {
        fprintf(stderr, "case %s: got %zu, %zu\n", name, lines, longest);
        exit(1);
    }
}

static void check_invalid(const char *name, const char *data, size_t n) {
    write_file(data, n);
    size_t lines, longest;
    errno = 0;
    if (line_stats(path, &lines, &longest) != -1 || errno != EILSEQ) {
        fprintf(stderr, "case %s: no EILSEQ error\n", name);
        exit(1);
    }
}

#define INVALID(name, data) check_invalid(name, data, sizeof(data) - 1)

int main(void) {
    CASE("lf", "alpha\nbeta\ngamma\n", 3, 5);
    CASE("blank-lines", "first\n\n\nlast line\n", 4, 9);
    CASE("empty", "", 0, 0);
    CASE("only-newline", "\n", 1, 0);
    CASE("no-final-newline", "one\ntwo\nthree", 3, 5);
    CASE("crlf", "one\r\ntwo\r\nthree\r\n", 3, 5);
    CASE("crlf-no-final-newline", "one\r\ntwo\r\nthree", 3, 5);
    CASE("mixed-endings", "one\r\ntwo\nthree\r\n", 3, 5);
    CASE("whitespace", "  two spaces\ttab \n", 1, 17);
    CASE("backslashes", "C:\\temp\\new\n", 1, 11);
    CASE("non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", 3, 10);

    size_t n = (size_t)1 << 20;
    char *long_line = malloc(n + 5);
    assert(long_line != NULL);
    memset(long_line, 'x', n);
    memcpy(long_line + n, "\nend\n", 5);
    check("long-line", long_line, n + 5, 2, n);
    free(long_line);

    INVALID("invalid-byte", "ok\n\xff\xfe\n");
    INVALID("truncated-at-end", "ok\ncaf\xc3");
    INVALID("encoded-surrogate", "a\xed\xa0\x80" "b\n");
    INVALID("overlong", "\xc0\xaf\n");

    size_t lines, longest;
    errno = 0;
    assert(line_stats("missing.txt", &lines, &longest) == -1 && errno == ENOENT);

    remove(path);
    puts("all tests passed");
    return 0;
}
