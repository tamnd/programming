// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

#include "first_and_last_lines.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "first_and_last_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

static void check_part(const char *name, const char *part, const struct line *got, size_t got_len,
                       const char *const *want, size_t n) {
    if (got_len != n) {
        fprintf(stderr, "case %s: the %s part has %zu lines, want %zu\n", name, part, got_len, n);
        exit(1);
    }
    for (size_t i = 0; i < n; i++) {
        if (got[i].len != strlen(want[i]) || strcmp(got[i].text, want[i]) != 0) {
            fprintf(stderr, "case %s: line %zu of the %s part is \"%s\", want \"%s\"\n", name, i, part, got[i].text,
                    want[i]);
            exit(1);
        }
    }
}

// The lines of the result, first and last, in one array: n first lines, then
// n last lines. Each case has the same number of lines in the two parts.
static void check(const char *name, const char *data, size_t size, size_t count, const char *const *want,
                  size_t n) {
    write_file(data, size);
    struct first_and_last got;
    if (first_and_last_lines(path, count, &got) != 0) {
        fprintf(stderr, "case %s: error %d\n", name, errno);
        exit(1);
    }
    check_part(name, "first", got.first, got.first_len, want, n);
    check_part(name, "last", got.last, got.last_len, n > 0 ? want + n : NULL, n);
    first_and_last_free(&got);
}

// sizeof counts the bytes of the literal, but not the final NUL. The first
// lines and then the last lines that the case wants come after the count.
#define CASE(name, data, count, ...)                                                          \
    do {                                                                                      \
        static const char *const want[] = {__VA_ARGS__};                                      \
        check(name, data, sizeof(data) - 1, count, want, sizeof want / sizeof want[0] / 2); \
    } while (0)

int main(void) {
    CASE("two", "a\nb\nc\nd\ne\n", 2, "a", "b", "d", "e");
    CASE("one", "first\nmiddle\nlast\n", 1, "first", "last");
    CASE("ten", "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n", 10, "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
         "3", "4", "5", "6", "7", "8", "9", "10", "11", "12");
    check("empty", "", 0, 2, NULL, 0);
    check("zero", "a\nb\n", 4, 0, NULL, 0);
    CASE("short", "a\nb\n", 5, "a", "b", "a", "b");
    CASE("exact", "a\nb\n", 2, "a", "b", "a", "b");
    CASE("overlap", "a\nb\nc\n", 2, "a", "b", "b", "c");
    CASE("one-line", "a\n", 1, "a", "a");
    CASE("empty-lines", "\n\na\n\n", 1, "", "");
    CASE("only-newline", "\n", 1, "", "");
    CASE("no-final-newline", "a\nb\nc", 1, "a", "c");
    CASE("crlf", "a\r\nb\r\n", 1, "a\r", "b\r");
    // U+00E9 is C3 A9, and U+00EB is C3 AB.
    CASE("non-ascii", "caf\xc3\xa9\nna\xc3\xafve\nzo\xc3\xab\n", 1, "caf\xc3\xa9", "zo\xc3\xab");

    // 100000 lines with the numbers 0 to 99999.
    size_t cap = 100000 * 6;
    char *data = malloc(cap);
    assert(data != NULL);
    size_t size = 0;
    for (int i = 0; i < 100000; i++) {
        size += (size_t)snprintf(data + size, cap - size, "%d\n", i);
    }
    check("many-lines", data, size, 3, (const char *const[]){"0", "1", "2", "99997", "99998", "99999"}, 3);
    free(data);

    struct first_and_last got;
    errno = 0;
    if (first_and_last_lines("no/such/dir/missing.txt", 1, &got) != -1 || errno != ENOENT) {
        fprintf(stderr, "case not-found: want -1 and ENOENT\n");
        return 1;
    }
    remove(path);
    puts("ok");
    return 0;
}
