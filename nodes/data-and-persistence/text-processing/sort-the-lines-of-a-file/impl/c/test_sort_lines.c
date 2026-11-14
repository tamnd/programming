// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

#include "sort_lines.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "sort_lines_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check(const char *name, const char *data, size_t size, const char *const *want, size_t n) {
    write_file(data, size);
    struct sorted_lines got;
    if (sort_lines(path, &got) != 0) {
        fprintf(stderr, "case %s: error %d\n", name, errno);
        exit(1);
    }
    if (got.len != n) {
        fprintf(stderr, "case %s: got %zu lines, want %zu\n", name, got.len, n);
        exit(1);
    }
    for (size_t i = 0; i < n; i++) {
        if (got.lines[i].len != strlen(want[i]) || strcmp(got.lines[i].text, want[i]) != 0) {
            fprintf(stderr, "case %s: line %zu is \"%s\", want \"%s\"\n", name, i, got.lines[i].text, want[i]);
            exit(1);
        }
    }
    sort_lines_free(&got);
}

// sizeof counts the bytes of the literal, but not the final NUL. The lines
// that the case wants come after the data.
#define CASE(name, data, ...)                                                        \
    do {                                                                             \
        static const char *const want[] = {__VA_ARGS__};                             \
        check(name, data, sizeof(data) - 1, want, sizeof want / sizeof want[0]);     \
    } while (0)

int main(void) {
    CASE("simple", "pear\napple\nfig\n", "apple", "fig", "pear");
    CASE("case", "b\nB\na\nA\n", "A", "B", "a", "b");
    CASE("numbers", "10\n9\n100\n", "10", "100", "9");
    check("empty", "", 0, NULL, 0);
    CASE("one-empty-line", "\n", "");
    CASE("empty-lines", "b\n\na\n\n", "", "", "a", "b");
    CASE("duplicates", "b\na\nb\n", "a", "b", "b");
    CASE("no-final-newline", "b\na", "a", "b");
    CASE("crlf", "b\r\na\r\n", "a\r", "b\r");
    CASE("prefix", "ab\na\n", "a", "ab");
    CASE("whitespace", "b\n a\n\tc\n", "\tc", " a", "b");
    // The UTF-8 bytes of U+00E9 are C3 A9, which are above the byte of z.
    CASE("non-ascii", "\xc3\xa9\nz\ne\n", "e", "z", "\xc3\xa9");
    // U+FF61 is EF BD A1 and U+1F600 is F0 9F 98 80.
    CASE("supplementary", "\xf0\x9f\x98\x80\n\xef\xbd\xa1\n", "\xef\xbd\xa1", "\xf0\x9f\x98\x80");
    // U+2028 is E2 80 A8.
    CASE("other-line-breaks", "b\xe2\x80\xa8" "a\nb\x1c" "a\nb\x0c" "a\n", "b\x0c" "a", "b\x1c" "a", "b\xe2\x80\xa8" "a");

    // 50000 lines "b" and 50000 lines "a", one after the other.
    size_t size = 4 * 50000;
    char *data = malloc(size);
    assert(data != NULL);
    for (size_t i = 0; i < size; i += 4) {
        memcpy(data + i, "b\na\n", 4);
    }
    write_file(data, size);
    free(data);
    struct sorted_lines got;
    assert(sort_lines(path, &got) == 0);
    assert(got.len == 100000);
    for (size_t i = 0; i < got.len; i++) {
        assert(strcmp(got.lines[i].text, i < 50000 ? "a" : "b") == 0);
    }
    sort_lines_free(&got);

    errno = 0;
    if (sort_lines("no/such/dir/missing.txt", &got) != -1 || errno != ENOENT) {
        fprintf(stderr, "case not-found: want -1 and ENOENT\n");
        return 1;
    }
    remove(path);
    puts("ok");
    return 0;
}
