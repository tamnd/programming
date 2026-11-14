// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

#include "read_line.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "read_line_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

// want is NULL if the case wants no line.
static void check(const char *name, const char *data, size_t size, int number, const char *want) {
    write_file(data, size);
    char *line = NULL;
    size_t len = 0;
    int got = read_line(path, number, &line, &len);
    if (got < 0) {
        fprintf(stderr, "case %s: error %d\n", name, errno);
        exit(1);
    }
    if (want == NULL ? got != 0 : got != 1 || len != strlen(want) || strcmp(line, want) != 0) {
        fprintf(stderr, "case %s: got %d \"%s\"\n", name, got, got == 1 ? line : "");
        exit(1);
    }
    free(line);
}

// sizeof counts the bytes of the literal, but not the final NUL.
#define CASE(name, data, number, want) check(name, data, sizeof(data) - 1, number, want)

int main(void) {
    CASE("first", "a\nb\nc\n", 1, "a");
    CASE("middle", "a\nb\nc\n", 2, "b");
    CASE("last", "a\nb\nc\n", 3, "c");
    CASE("past-the-end", "a\nb\n", 3, NULL);
    CASE("zero", "a\n", 0, NULL);
    CASE("negative", "a\n", -1, NULL);
    CASE("empty", "", 1, NULL);
    CASE("empty-line", "a\n\nb\n", 2, "");
    CASE("only-newline", "\n", 1, "");
    CASE("no-final-newline", "a\nb", 2, "b");
    CASE("crlf", "a\r\nb\r\n", 1, "a\r");
    CASE("lone-cr", "a\rb\nc\n", 2, "c");
    // U+00E9 is C3 A9, and U+00EB is C3 AB.
    CASE("non-ascii", "caf\xc3\xa9\nzo\xc3\xab\n", 2, "zo\xc3\xab");

    // 100000 lines with the numbers 0 to 99999. Line n holds n - 1.
    size_t cap = 100000 * 6;
    char *data = malloc(cap);
    assert(data != NULL);
    size_t size = 0;
    for (int i = 0; i < 100000; i++) {
        size += (size_t)snprintf(data + size, cap - size, "%d\n", i);
    }
    check("many-lines", data, size, 100000, "99999");
    check("many-lines-past-the-end", data, size, 100001, NULL);
    free(data);

    char *line;
    size_t len;
    errno = 0;
    if (read_line("no/such/dir/missing.txt", 1, &line, &len) != -1 || errno != ENOENT) {
        fprintf(stderr, "case not-found: want -1 and ENOENT\n");
        return 1;
    }
    remove(path);
    puts("ok");
    return 0;
}
