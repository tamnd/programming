// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

#include "remove_duplicate_lines.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "remove_duplicate_lines_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check(const char *name, const char *data, size_t size, const char *const *want, size_t n) {
    write_file(data, size);
    struct unique_lines got;
    if (remove_duplicate_lines(path, &got) != 0) {
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
    unique_lines_free(&got);
}

// sizeof counts the bytes of the literal, but not the final NUL. The lines
// that the case wants come after the data.
#define CASE(name, data, ...)                                                    \
    do {                                                                         \
        static const char *const want[] = {__VA_ARGS__};                         \
        check(name, data, sizeof(data) - 1, want, sizeof want / sizeof want[0]); \
    } while (0)

int main(void) {
    CASE("simple", "b\na\nb\nc\na\n", "b", "a", "c");
    CASE("adjacent", "a\na\nb\n", "a", "b");
    CASE("no-duplicates", "c\nb\na\n", "c", "b", "a");
    check("empty", "", 0, NULL, 0);
    CASE("empty-lines", "\na\n\n\n", "", "a");
    CASE("all-same", "x\nx\nx\n", "x");
    CASE("no-final-newline", "a\nb\na", "a", "b");
    CASE("crlf", "a\r\na\n", "a\r", "a");
    CASE("case", "A\na\nA\n", "A", "a");
    CASE("whitespace", "a \na\n a\na \n", "a ", "a", " a");
    // U+00E9 is C3 A9. e and U+0301 is 65 CC 81.
    CASE("normalization", "\xc3\xa9\ne\xcc\x81\n\xc3\xa9\n", "\xc3\xa9", "e\xcc\x81");

    // 100000 lines with 1000 different lines make the table grow more than once.
    char *data = malloc(100000 * 4);
    assert(data != NULL);
    size_t size = 0;
    for (int i = 0; i < 100000; i++) {
        size += (size_t)snprintf(data + size, 100000 * 4 - size, "%d\n", i % 1000);
    }
    write_file(data, size);
    free(data);
    struct unique_lines got;
    assert(remove_duplicate_lines(path, &got) == 0);
    assert(got.len == 1000);
    for (size_t i = 0; i < got.len; i++) {
        char want[24];
        snprintf(want, sizeof want, "%zu", i);
        assert(strcmp(got.lines[i].text, want) == 0);
    }
    unique_lines_free(&got);

    errno = 0;
    if (remove_duplicate_lines("no/such/dir/missing.txt", &got) != -1 || errno != ENOENT) {
        fprintf(stderr, "case not-found: want -1 and ENOENT\n");
        return 1;
    }
    remove(path);
    puts("ok");
    return 0;
}
