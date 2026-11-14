// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-lines-that-match-a-pattern/.

#include "filter_lines.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "filter_lines_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check(const char *name, const char *data, size_t size, const char *pattern, const char *const *want,
                  size_t n) {
    write_file(data, size);
    struct matched_lines got;
    if (filter_lines(path, pattern, &got) != 0) {
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
    matched_lines_free(&got);
}

// sizeof counts the bytes of the literal, but not the final NUL. The lines
// that the case wants come after the pattern.
#define CASE(name, data, pattern, ...)                                                    \
    do {                                                                                  \
        static const char *const want[] = {__VA_ARGS__};                                  \
        check(name, data, sizeof(data) - 1, pattern, want, sizeof want / sizeof want[0]); \
    } while (0)

int main(void) {
    CASE("literal", "apple\nbanana\ncherry\n", "an", "banana");
    CASE("several", "cat\ndog\ncatalog\n", "cat", "cat", "catalog");
    CASE("start", "ab\nba\n", "^a", "ab");
    CASE("end", "ab\nba\n", "a$", "ba");
    CASE("dot", "abc\nac\naxc\n", "a.c", "abc", "axc");
    CASE("star", "ac\nabc\nabbc\nadc\n", "ab*c", "ac", "abc", "abbc");
    CASE("whole-line", "a\nab\nba\n", "^a$", "a");
    check("empty", "", 0, "a", NULL, 0);
    check("no-match", "abc\n", 4, "x", NULL, 0);
    CASE("empty-lines", "a\n\nb\n\n", "^$", "", "");
    CASE("match-all", "a\n\nb\n", ".*", "a", "", "b");
    CASE("duplicates", "a\nb\na\n", "a", "a", "a");
    CASE("no-final-newline", "a\nab", "b", "ab");
    CASE("crlf-end", "ab\r\nab\n", "b$", "ab");
    CASE("crlf-dot", "a\r\n", "^a.$", "a\r");
    CASE("case", "Apple\napple\n", "apple", "apple");
    // U+00E9 is C3 A9.
    CASE("non-ascii", "caf\xc3\xa9\ncafe\n", "\xc3\xa9", "caf\xc3\xa9");

    // 100000 lines with the numbers 0 to 99999. Five of them hold only nines.
    size_t cap = 100000 * 6;
    char *data = malloc(cap);
    assert(data != NULL);
    size_t size = 0;
    for (int i = 0; i < 100000; i++) {
        size += (size_t)snprintf(data + size, cap - size, "%d\n", i);
    }
    write_file(data, size);
    free(data);
    struct matched_lines got;
    assert(filter_lines(path, "^9*$", &got) == 0);
    const char *nines[] = {"9", "99", "999", "9999", "99999"};
    assert(got.len == 5);
    for (size_t i = 0; i < got.len; i++) {
        assert(strcmp(got.lines[i].text, nines[i]) == 0);
    }
    matched_lines_free(&got);

    errno = 0;
    if (filter_lines("no/such/dir/missing.txt", "a", &got) != -1 || errno != ENOENT) {
        fprintf(stderr, "case not-found: want -1 and ENOENT\n");
        return 1;
    }
    remove(path);
    puts("ok");
    return 0;
}
