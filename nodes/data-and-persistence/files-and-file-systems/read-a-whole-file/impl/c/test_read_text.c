// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

#include "read_text.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "read_text_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check(const char *name, const char *data, size_t n) {
    write_file(data, n);
    size_t len = 99;
    char *text = read_text(path, &len);
    if (text == NULL || len != n || memcmp(text, data, n) != 0 || text[n] != '\0') {
        fprintf(stderr, "case %s: wrong text\n", name);
        exit(1);
    }
    free(text);
}

static void check_error(const char *name, const char *file, int want) {
    size_t len;
    errno = 0;
    char *text = read_text(file, &len);
    if (text != NULL || errno != want) {
        fprintf(stderr, "case %s: want errno %d, got %d\n", name, want, errno);
        exit(1);
    }
}

static void check_invalid(const char *name, const char *data, size_t n) {
    write_file(data, n);
    check_error(name, path, EILSEQ);
}

#define CASE(name, data) check(name, data, sizeof(data) - 1)
#define INVALID(name, data) check_invalid(name, data, sizeof(data) - 1)

int main(void) {
    CASE("lf", "alpha\nbeta\n");
    CASE("paragraphs", "first line\n\nsecond paragraph\n");
    CASE("non-ascii", "na\xc3\xafve caf\xc3\xa9\n\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\n\xf0\x9f\x99\x82\n");
    CASE("empty", "");
    CASE("no-final-newline", "one\ntwo");
    CASE("trailing-newlines", "end\n\n\n");
    CASE("crlf", "one\r\ntwo\r\n");
    CASE("mixed-endings", "lf\ncrlf\r\ncr\rend");
    CASE("bom", "\xef\xbb\xbfhead\n");
    CASE("whitespace", "  two spaces\ttab \n");

    size_t big = 16 * 65536;
    char *large = malloc(big);
    assert(large != NULL);
    for (size_t i = 0; i < big; i++) large[i] = "0123456789abcdef"[i % 16];
    check("large", large, big);
    free(large);

    remove(path);
    check_error("missing", "no_such_file.txt", ENOENT);
    check_error("directory", ".", EISDIR);

    INVALID("invalid-byte", "ok\n\xff\xfe\n");
    INVALID("truncated-at-end", "ok\ncaf\xc3");
    INVALID("encoded-surrogate", "a\xed\xa0\x80" "b\n");
    INVALID("overlong", "\xc0\xaf\n");
    remove(path);

    puts("all tests passed");
    return 0;
}
